// https://hdlbits.01xz.net/wiki/Lemmings4

module top_module(
    input clk,
    input areset,    // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    input ground,
    input dig,
    output walk_left,
    output walk_right,
    output aaah,
    output digging ); 

    parameter [2:0] LEFT=3'b000, RIGHT=3'b001, FALL=3'b010, DIG=3'b011, STOP=3'b100;
    reg [2:0] state, next_state, hori_state;

	wire [4:0] shamt = 5'd20;

	reg [4:0] count;
    always @(posedge clk, posedge areset) begin
		if (areset) begin
			count <= 5'd0;
		end else begin
			if (state == FALL) begin
				if (count == shamt)
					count <= count;
				else 
					count <= count + 5'd1;
			end else begin
				count <= 5'd0;
			end
		end
	end

    always @(*) begin
        case (state)
			STOP : next_state = STOP;
			FALL : begin
				if (ground) begin
					if (count == shamt)
						next_state = STOP;
					else
						next_state = hori_state;
				end else begin
					next_state = FALL;
				end
			end
			DIG  : next_state = ground ? DIG : FALL;
            LEFT : next_state = ground ? (dig ? DIG : (bump_left ? RIGHT : LEFT)) : FALL;
            RIGHT: next_state = ground ? (dig ? DIG : (bump_right ? LEFT : RIGHT)): FALL;
			default: next_state = STOP;
        endcase
    end

	always @(posedge clk) begin
        case (state)
            LEFT : hori_state <= LEFT;
            RIGHT: hori_state <= RIGHT;
            default: hori_state <= hori_state;
        endcase
    end

    always @(posedge clk, posedge areset) begin
        if (areset) 
            state <= LEFT;
        else
            state <= next_state;
    end

    assign walk_left = (state == LEFT) ? 1 : 0;
    assign walk_right = (state == RIGHT) ? 1 : 0;
	assign aaah = (state == FALL) ? 1 : 0;
	assign digging = (state == DIG) ? 1 : 0;

endmodule
