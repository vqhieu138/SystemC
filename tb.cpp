#include "tb.h"

sc_int<11> cnt = 0;
sc_int<32> inp_wire = 0;
void tb::source() {
	//cout << "source function here" << endl;
	rst.write(0);
	o_tready.write(1);
	wait(5,SC_NS);
	rst.write(1);
	
	while (true) {
		
		if (!rst.read()) {
			inp.write(0);
			cnt = 0;
		}
		else {
			if (cnt < 1000) {
				i_tvalid.write(1);
				inp_wire.range(15,0) = data_lut[cnt];
				inp_wire.range(31,16) = data_lut[cnt+1];
				inp.write(inp_wire);
				i_tlast.write(0);
				cnt = cnt + 2;
			}
			else if (cnt == 1000) {
				i_tvalid.write(1);
				inp_wire.range(15,0) = data_lut[cnt];
				inp_wire.range(31,16) = data_lut[cnt+1];
				inp.write(inp_wire);
				i_tlast.write(1);
				cnt = cnt + 2;
			} else {
				i_tvalid.write(0);
				inp.write(0);
				i_tlast.write(0);
			}
		}
		wait();
	}
}

void tb::sink() {
	int out;
	

	//cout << "sink function here" << endl;
	while (true) {
		//out = outp_lp.read();
		//cout << "@" << sc_time_stamp() << " Asserting Enable\n" << endl;
		if (o_tlast.read()) {
			
			//cout << outp.read() << endl;
			//printf("%x -- %d", out, cnt);
			std::cout << std::hex << outp_lp.read() << " " << outp_hp.read() << std::endl;
			//sc_close_vcd_trace_file(wf);
			wait(20,SC_NS);
			sc_stop();
		}
		else if (o_tvalid.read()) {
			std::cout  << std::hex << outp_lp.read() << " " << outp_hp.read() << std::endl;
			//cout << cnt << "   " << outp.read() << endl;
			//printf("%x -- %d", out, cnt);
		}
		wait();
	}
}
