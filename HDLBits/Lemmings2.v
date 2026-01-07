// https://hdlbits.01xz.net/wiki/Lemmings2

module top_module(
    input clk,
    input areset,    // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    input ground,
    output walk_left,
    output walk_right,
    output aaah ); 

    parameter [1:0] LEFT=2'b00, RIGHT=2'b01, DOWN=2'b10;
    reg [1:0] state, next_state, hori_state;

    always @(*) begin
        case (state)
			DOWN : next_state = ground ? hori_state : DOWN;
            LEFT : next_state = ~ground ? DOWN : (bump_left  ? RIGHT : LEFT);
            RIGHT: next_state = ~ground ? DOWN : (bump_right ? LEFT : RIGHT);
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
	assign aaah = (state == DOWN) ? 1 : 0;

endmodule
