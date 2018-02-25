module simple (
CLKIN,
DATA1,
DOUT1
);

// Start PIs
input CLKIN;
input DATA1;

// Start POs
output DOUT1;

// Start cells
TOP u1 ( .CLKIN(CLKIN), .DATA1(DATA1), .DOUT1(DOUT1) );

endmodule
