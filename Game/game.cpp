#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define pressed(b) (!input->buttons[b].is_down && input->buttons[b].changed)


float player_p, player_dp;
float player1_p,player1_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x=130, ball_dp_y,ball_half_size = 1,max_ball_dp=300;

int player_score = 0, player1_score = 0;

enum GameMode {
	GM_MENU,
	GM_GAMEPLAY,
};

GameMode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void
simulate_player(float *p, float *dp, float ddp, float dt)
{
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y)
	{
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else
		if (*p - player_half_size_y < -arena_half_size_y)
		{
			*p = -arena_half_size_y + player_half_size_y;
			*dp = 0;
		}
}


internal void
simulate_game(Input* input, float dt)
{
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffbe98);
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xf05a7e);


	if (current_gamemode == GM_GAMEPLAY) {

		float player_ddp = 0.f;
		if (!enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_ddp += 1500;
			if (is_down(BUTTON_DOWN)) player_ddp -= 1500;
		}
		else
		{
			player_ddp = (ball_p_y - player_p) * 100;
			if (player_ddp > 900) player_ddp = 900;
			if (player_ddp < -900) player_ddp = -900;
		}

		float player1_ddp = 0.f;

		if (is_down(BUTTON_W)) player1_ddp += 1500;
		if (is_down(BUTTON_S)) player1_ddp -= 1500;

		simulate_player(&player_p, &player_dp, player_ddp, dt);
		simulate_player(&player1_p, &player1_dp, player1_ddp, dt);
		//Simulate Ball
		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;

			if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
				ball_p_x - ball_half_size < 80 + player_half_size_x &&
				ball_p_y + ball_half_size > player_p - player_half_size_y &&
				ball_p_y + ball_half_size < player_p + player_half_size_y) {
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				if (ball_dp_x * 1.05 < max_ball_dp)
					ball_dp_x *= -1.05;
				else
					ball_dp_x = -max_ball_dp;
				ball_dp_y = (ball_p_y - player_p) * 2 + player_dp * .5f;
			}
			else
				if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
					ball_p_x - ball_half_size < -80 + player_half_size_x &&
					ball_p_y + ball_half_size > player1_p - player_half_size_y &&
					ball_p_y + ball_half_size < player1_p + player_half_size_y) {
					ball_p_x = -80 + player_half_size_x + ball_half_size;
					if (ball_dp_x * 1.05 > -max_ball_dp)
						ball_dp_x *= -1.05;
					else
						ball_dp_x = max_ball_dp;
					ball_dp_y = (ball_p_y - player1_p) * 2 + player1_dp * .5f;
				}
			if (ball_p_y + ball_half_size > arena_half_size_y)
			{
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else
				if (ball_p_y - ball_half_size < -arena_half_size_y)
				{
					ball_p_y = -arena_half_size_y + ball_half_size;
					ball_dp_y *= -1;
				}

			if (ball_p_x + ball_half_size > arena_half_size_x)
			{
				ball_dp_x = 100;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_score++;
			}
			if (ball_p_x - ball_half_size < -arena_half_size_x)
			{
				ball_dp_x = -100;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player1_score++;
			}

		}
		draw_number(player_score, -10, 40, 1.f, 0xf05a7e);
		draw_number(player1_score, 10, 40, 1.f, 0xf05a7e);

		draw_rect(ball_p_x, ball_p_y, 1, 1, 0xb8494);

		draw_rect(80, player_p, player_half_size_x, player_half_size_y, 0x125b9a);
		draw_rect(-80, player1_p, player_half_size_x, player_half_size_y, 0x125b9a);
	}
	else
	{
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
		{
			hot_button = ~hot_button;
		}

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 1 : 0;
		}

		if (hot_button == 0)
		{
			draw_text("SINGLE PLAYER", -83, -10, 1, 0xf05a7e);
			draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
		}
		else
		{
			draw_text("SINGLE PLAYER", -83, -10, 1, 0xaaaaaa);
			draw_text("MULTIPLAYER", 20, -10, 1, 0xf05a7e);
		}
		draw_text("SPEED PONG", -60, 40, 2, 0xf05a7e);
	}

	

}