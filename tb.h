#include "systemc.h"
#include "./data_lut.h"


SC_MODULE(tb) {
	sc_out <bool> rst;
	sc_in<bool> clk;
	sc_out <sc_int<32> > inp;
	sc_out <bool> i_tvalid;
	sc_out<bool> i_tlast;

	sc_in<sc_int<16> > outp_lp;
	sc_in<sc_int<16> > outp_hp;
	sc_out<bool> o_tready;
	sc_in<bool> o_tvalid;
	sc_in<bool> o_tlast;

	void source();
	void sink();

	SC_CTOR(tb) {
		SC_THREAD(source);
		sensitive << clk.pos();
		sensitive << rst;
		//reset_signal_is(rst, false);

		SC_THREAD(sink);
		sensitive << clk.pos();
		sensitive << rst;
		//reset_signal_is(rst, false);
	}
};

