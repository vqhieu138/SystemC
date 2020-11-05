#include "system.h"
#include "systemc.h"


int sc_main(int argc, char* argv[]) {
	SYSTEM* top;
	top = new SYSTEM("top");
	//cout << "source function here" << endl;
	//sc_trace_file *wf = sc_create_vcd_trace_file("conv");
    // Dump the desired signals
    
	//sc_trace(wf,(*top).dut_0.data_cnt,"data_cnt");
	//(*top).rst.write(0);

	//sc_start(5, SC_NS);
	//(*top).rst.write(1);
	sc_start();
	return 0;// Terminate simulation

}

