// https://nju-projectn.github.io/dlco-lecture-note/exp/07.html#id10

// https://blog.csdn.net/m0_51209350/article/details/109237270
// https://blog.csdn.net/liuyukuan/article/details/120575746

`timescale 1s / 1s

`include "mux_template.v"

module ex7(
    input clk,
    input clrn,
    input kbd_clk,
    input kbd_data,
    output ready, overflow, sampling,
    output [7:0] ps2_out,
    output [7:0] ascii_out,
    output reg [6:0] seg_count1, seg_count0, 
    output reg [6:0] seg_ascii1, seg_ascii0,
    output reg [6:0] seg_scan_code1, seg_scan_code0,
    output reg [6:0] seg_empty1, seg_empty0
);
    parameter [7:0] break_code = 8'hF0;

    // wire [7:0] ps2_out;
    reg [7:0] scan_code;
    // reg [7:0] ascii_out;

    reg [7:0] count_out;

    reg nextdata_n;
    always @(posedge clk) begin
        if (clrn == 0)
            nextdata_n <= 1'b0;
        else begin
            // if (nextdata_n == 1'b0)
            //     nextdata_n <= 1'b1;
            // else begin
                if (ready)
                    nextdata_n <= 1'b0;
                else
                    nextdata_n <= 1'b1;
            // end
        end
    end

    // 接收键盘数据
    ps2_keyboard inst_ps2(
        .clk(clk),
        .clrn(clrn),
        .ps2_clk(kbd_clk),
        .ps2_data(kbd_data),
        .nextdata_n(nextdata_n),
        .data(ps2_out),
        .sampling(sampling),
        .ready(ready),
        .overflow(overflow)
    );

    // 处理键盘数据
    always @(posedge clk) begin
        if (ready) begin
            scan_code <= ps2_out;
        end else begin
            scan_code <= scan_code;
        end
    end

    // 识别按键ascii
    get_ascii_2 inst_ascii(
        .scan_code(scan_code),
        .ascii_out(ascii_out)
    );

    // 计算按键总次数, 按住不放只算一次

    // always @(posedge clk) begin 
    //     if (clrn == 0) begin
    //         count_out <= 8'h00;
    //     end else begin
    //         if (scan_code == break_code)
    //             count_out <= count_out + 8'h01;
    //         else
    //             count_out <= count_out;
    //     end
    // end

    reg in_break_seq; // 标记是否处于break序列中
    always @(posedge clk) begin
        if (clrn == 0) begin
            in_break_seq <= 1'b0;
            count_out <= 8'h00;
        end else begin
            if (scan_code == break_code) begin
                in_break_seq <= 1'b1; // 开始break序列
            end else if (in_break_seq) begin
                in_break_seq <= 1'b0; // 结束break序列，计数加1
                count_out <= count_out + 8'h01;
            end
        end
    end
    	
    // 按键的数码管显示
    light_seg_x show_scan_code0 (
        .digit(scan_code[3:0]),
        .ready(1'b1),
        .seg(seg_scan_code0)
    );
    light_seg_x show_scan_code1 (
        .digit(scan_code[7:4]),
        .ready(1'b1),
        .seg(seg_scan_code1)
    );

    light_seg_x show_ascii0 (
        .digit(ascii_out[3:0]),
        .ready(1'b1),
        .seg(seg_ascii0)
    );
    light_seg_x show_ascii1 (
        .digit(ascii_out[7:4]),
        .ready(1'b1),
        .seg(seg_ascii1)
    );

    light_seg_x show_count0 (
        .digit(count_out[3:0]),
        .ready(1'b1),
        .seg(seg_count0)
    );
    light_seg_x show_count1 (
        .digit(count_out[7:4]),
        .ready(1'b1),
        .seg(seg_count1)
    );

    assign seg_empty0 = 7'b111_1111;
    assign seg_empty1 = 7'b111_1111;
endmodule

// IBM PC AT
module get_ascii_2(
    input [7:0] scan_code,
    output reg [7:0] ascii_out
); 
    MuxKey #(40, 8, 8) scancode2ascii (ascii_out, scan_code, {
        // 字母和空格
        8'h1C, 8'h61,
        8'h32, 8'h62,
        8'h21, 8'h63,
        8'h23, 8'h64,
        8'h24, 8'h65,
        8'h2B, 8'h66,
        8'h34, 8'h67,
        8'h33, 8'h68,
        8'h43, 8'h69,
        8'h3B, 8'h6A,
        8'h42, 8'h6B,
        8'h4B, 8'h6C,
        8'h3A, 8'h6D,
        8'h31, 8'h6E,
        8'h44, 8'h6F,
        8'h4D, 8'h70,
        8'h15, 8'h71,
        8'h2D, 8'h72,
        8'h1B, 8'h73,
        8'h2C, 8'h74,
        8'h3C, 8'h75,
        8'h2A, 8'h76,
        8'h1D, 8'h77,
        8'h22, 8'h78,
        8'h35, 8'h79,
        8'h1A, 8'h7A,
        8'h29, 8'h20,

        // 第一排数字键盘
        8'h0E, 8'h60,
        8'h16, 8'h31,
        8'h1E, 8'h32,
        8'h26, 8'h33,
        8'h25, 8'h34,
        8'h2E, 8'h35,
        8'h36, 8'h36,
        8'h3D, 8'h37,
        8'h3E, 8'h38,
        8'h46, 8'h39,
        8'h45, 8'h30,
        8'h4E, 8'h2D,
        8'h55, 8'h3D
    });

endmodule

// IBM PC XT
module get_ascii_1(
    input [7:0] scan_code,
    output reg [7:0] ascii_out
); 
    MuxKey #(48, 8, 8) scancode2ascii (ascii_out, scan_code, {
        // 字母和空格
        8'h1E, 8'h61,
        8'h30, 8'h62,
        8'h2E, 8'h63,
        8'h20, 8'h64,
        8'h12, 8'h65,
        8'h21, 8'h66,
        8'h22, 8'h67,
        8'h23, 8'h68,
        8'h17, 8'h69,
        8'h24, 8'h6A,
        8'h25, 8'h6B,
        8'h26, 8'h6C,
        8'h32, 8'h6D,
        8'h31, 8'h6E,
        8'h18, 8'h6F,
        8'h19, 8'h70,
        8'h10, 8'h71,
        8'h13, 8'h72,
        8'h1F, 8'h73,
        8'h14, 8'h74,
        8'h16, 8'h75,
        8'h2F, 8'h76,
        8'h11, 8'h77,
        8'h2D, 8'h78,
        8'h15, 8'h79,
        8'h2C, 8'h7A,
        8'h39, 8'h20,

        // 第一排数字键盘
        8'h29, 8'h60,
        8'h02, 8'h31,
        8'h03, 8'h32,
        8'h04, 8'h33,
        8'h05, 8'h34,
        8'h06, 8'h35,
        8'h07, 8'h36,
        8'h08, 8'h37,
        8'h09, 8'h38,
        8'h0A, 8'h39,
        8'h0B, 8'h30,
        8'h0C, 8'h2D,
        8'h0D, 8'h3D,

        // 数字小键盘
    
        // 标号按键
        8'h1A, 8'h5B,
        8'h1B, 8'h5D,
        8'h27, 8'h3B,
        8'h28, 8'h27,
        8'h2b, 8'h5C,
        8'h33, 8'h2C,
        8'h34, 8'h2E,
        8'h35, 8'h2F
    });
endmodule

module light_seg_x (
    input [3:0] digit,
    input ready,
    output [6:0] seg
);
    MuxKeyWithDefault #(16, 5, 7) digit2hex (seg, {ready, digit}, 7'b111_1111, {
        5'h10, 7'b000_0001,
        5'h11, 7'b100_1111,
        5'h12, 7'b001_0010,
        5'h13, 7'b000_0110,

        5'h14, 7'b100_1100,
        5'h15, 7'b010_0100,
        5'h16, 7'b010_0000,
        5'h17, 7'b000_1101,

        5'h18, 7'b000_0000,
        5'h19, 7'b000_0100,
        5'h1a, 7'b000_1000,
        5'h1b, 7'b110_0000,

        5'h1c, 7'b011_0001,
        5'h1d, 7'b100_0010,
        5'h1e, 7'b011_0000,
        5'h1f, 7'b011_1000
    });
endmodule

// 键盘控制器模块
module ps2_keyboard(
    input clk, clrn,
    input ps2_clk, ps2_data,
    input nextdata_n,
    output [7:0] data,
    output sampling,
    output reg ready,
    output reg overflow     // fifo overflow
);
    // internal signal, for test
    reg [9:0] buffer;        // ps2_data bits
    reg [7:0] fifo[7:0];     // data fifo
    reg [2:0] w_ptr, r_ptr;   // fifo write and read pointers
    reg [3:0] count;  // count ps2_data bits

    // detect falling edge of ps2_clk
    reg [2:0] ps2_clk_sync;

    always @(posedge clk) begin
        ps2_clk_sync <= {ps2_clk_sync[1:0], ps2_clk};
    end

    assign sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    always @(posedge clk) begin
        if (clrn == 0) begin // reset
            count <= 0; 
            w_ptr <= 0; r_ptr <= 0; 
            overflow <= 0; ready <= 0;
        end
        else begin
            if ( ready ) begin // read to output next data
                if(nextdata_n == 1'b0) //read next data
                begin
                    r_ptr <= r_ptr + 3'b1;
                    if(w_ptr == (r_ptr + 1'b1)) //empty
                        ready <= 1'b0;
                end
            end
            if ( sampling ) begin
                if (count == 4'd10) begin
                    if ((buffer[0] == 0) &&  // start bit
                        (ps2_data)       &&  // stop bit
                        (^buffer[9:1])       // odd parity
                    ) begin      
                        fifo[w_ptr] <= buffer[8:1];  // kbd scan code
                        w_ptr <= w_ptr + 3'b1;
                        ready <= 1'b1;
                        overflow <= overflow | (r_ptr == (w_ptr + 3'b1));
                    end
                    count <= 0;     // for next
                end else begin
                    buffer[count] <= ps2_data;  // store ps2_data
                    count <= count + 3'b1;
                end
            end
        end
    end

    assign data = fifo[r_ptr]; // always set output data
endmodule
