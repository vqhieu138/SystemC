
#include "systemc.h"

#define FW 8
#define N 2
const sc_int<16> coefs_lp[8] = { 0xf64d, 0xfc35, 0x3fb2, 0x66e1, 0x2620, 0xf34d, 0xfe63, 0x0420 };
const sc_int<16> coefs_hp[8] = { 0xfbe0, 0xfe63, 0x0cb3, 0x2620, 0x991f, 0x3fb2, 0x03cb, 0xf64d  };

SC_MODULE(fir) {
	sc_in <bool> clk, rst;
	sc_in <sc_int<32> > inp_lp;
	sc_in <sc_int<32> > inp_hp;
	sc_in <bool> i_tvalid;
	sc_in<bool> i_tlast;

	sc_out <sc_int<16> > outp_lp;
	sc_out <sc_int<16> > outp_hp;

	sc_in <bool> o_tready;
	sc_out <bool> o_tvalid;
	sc_out <bool> o_tlast;
	sc_signal <sc_int<5> > data_cnt;
	sc_signal <sc_int<16> > taps_lp[8];
	sc_signal <sc_int<16> > taps_hp[8];

	void peripheral_control();
	void o_tlast_contol();
	void o_tvalid_control();

	void fir_main();
	void shift_data();
	void load_data();
	void output_data();

	SC_CTOR(fir)
	:clk("clk"),
	rst("rst"),
	inp_lp("inp_lp"),
	inp_hp("inp_hp"),
	i_tvalid("i_tvalid"),
	i_tlast("i_tlast"),
	outp_lp("outp_lp"),
	outp_hp("outp_hp"),
	o_tvalid("o_tvalid"),
	o_tlast("o_tlast"),
	data_cnt("data_cnt") {
		SC_CTHREAD(peripheral_control, clk.pos());
		async_reset_signal_is(rst, false);
		SC_CTHREAD(o_tlast_contol, clk.pos());
		async_reset_signal_is(rst, false);
		SC_CTHREAD(o_tvalid_control, clk.pos());
		async_reset_signal_is(rst, false);
		SC_CTHREAD(load_data, clk.pos());
		async_reset_signal_is(rst, false);
		SC_CTHREAD(shift_data,clk.pos());
		async_reset_signal_is(rst,false);
		SC_METHOD(output_data)
		for (int i = 0; i < FW; i ++)
			sensitive << taps_hp[i] << taps_lp[i];
	}
};
