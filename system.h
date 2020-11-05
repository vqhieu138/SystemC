#include "systemc.h"
#include "tb.h"
#include "fir.h"

SC_MODULE(SYSTEM) {
    sc_signal<bool> rst;
    sc_signal<sc_int<32> > inp;
    sc_signal<bool> i_tvalid;
    sc_signal<bool> i_tlast;

    sc_signal<sc_int<16> > outp_lp;
    sc_signal<sc_int<16> > outp_hp;
    sc_signal<bool> o_tvalid;
    sc_signal<bool> o_tlast;
    sc_signal<bool> o_tready;

    sc_clock clk;

    fir dut_0;
    tb tb_0;

    SC_CTOR(SYSTEM)
        :clk("clk", 1, 0.5, 0, true),
        dut_0("dut_0"),
        tb_0("tb_0")
    {
        dut_0.clk(clk);
        dut_0.rst(rst);
        dut_0.inp_lp(inp);
        dut_0.inp_hp(inp);
        dut_0.outp_lp(outp_lp);
        dut_0.outp_hp(outp_hp);
        dut_0.o_tready(o_tready);
        dut_0.i_tlast(i_tlast);
        dut_0.i_tvalid(i_tvalid);
        dut_0.o_tvalid(o_tvalid);
        dut_0.o_tlast(o_tlast);

        tb_0.clk(clk);
        tb_0.rst(rst);
        tb_0.inp(inp);
        tb_0.outp_hp(outp_hp);
        tb_0.outp_lp(outp_lp);
        tb_0.o_tready(o_tready);
        tb_0.i_tlast(i_tlast);
        tb_0.i_tvalid(i_tvalid);
        tb_0.o_tvalid(o_tvalid);
        tb_0.o_tlast(o_tlast);
    }
};
