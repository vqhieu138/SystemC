#include "DUT.h"
#include "systemc.h"


void fir::o_tlast_contol() {
	o_tlast.write(0);
	wait();

	while(1) {
		if (o_tvalid.read() && (data_cnt.read() == FW/N - 1) && !o_tlast.read() && o_tready.read())
			o_tlast.write(1);
		else if (o_tlast.read())
			o_tlast.write(0);
		wait();
	}
}

void fir::o_tvalid_control() {
	o_tvalid.write(0);
	wait();

	while(1) {
		if(o_tready.read())	
			if (!i_tlast.read() && data_cnt.read() == 0)
				o_tvalid.write(i_tvalid.read());
			else if (data_cnt.read() == FW/N)
				o_tvalid.write(0);
		wait();
	}
}


void fir::peripheral_control() {
	data_cnt = 0;
	wait();
	
	while (true) {
		if (o_tready.read())
			if (i_tlast.read() || data_cnt.read() != 0)
				data_cnt.write(data_cnt.read() + 1);
		wait();
	}
}

void fir::shift_data() {
	for (int i = N; i < FW; i++) {
		taps_lp[i].write(0);
		taps_hp[i].write(0);
	}
	wait();

	while(1) {
		if(o_tready.read())
			if (i_tvalid.read() || data_cnt.read() != 0) {
				for (int i = 1; i < FW - N + 1; i++) {
					taps_hp[FW - i].write(taps_hp[FW - i - N].read());
					taps_lp[FW - i].write(taps_lp[FW - i - N].read());
				}
			}
		wait();
	}
}

void fir::load_data() {
	sc_int<32> inp_wire_hp;
	sc_int<32> inp_wire_lp;
	for (int i = 0; i < N; i++) {
		taps_hp[i].write(0);
		taps_lp[i].write(0);
	}
    wait();
	
	while(1) {
		if(o_tready.read())	
			if (i_tvalid.read()) {
				inp_wire_hp = inp_hp.read();
				inp_wire_lp = inp_lp.read();
				for(int i = 0; i < N; i ++) {
					taps_hp[i].write(inp_wire_hp.range(16*N - 16*i -1,16*N - 16*i -1 -15));
					taps_lp[i].write(inp_wire_lp.range(16*N - 16*i -1,16*N - 16*i -1 -15));
				}
			} else if (data_cnt.read() || i_tlast.read()) {
				for(int i = 0; i < N; i ++) {
					taps_hp[i].write(0);
					taps_lp[i].write(0);
				}
			}
		wait();
	}
}

void fir::output_data() {
	sc_int<32> val_lp = 0;
	sc_int<32> val_hp =0;
	//outp.write(0);
	val_lp = 0;
	val_hp = 0;
	for (int i = 0; i < FW; i++) {
		val_hp += coefs_hp[i] * taps_hp[i].read();
		val_lp += coefs_lp[i] * taps_lp[i].read();
	}
	outp_lp.write(val_lp.range(31,16));
	outp_hp.write(val_hp.range(31,16));
}
