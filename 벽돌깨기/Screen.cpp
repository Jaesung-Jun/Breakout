#include "Screen.h"

void Screen::Print_Score_Board_Info(DoubleBuffering* dbuff, string str, COORD score_box_xy, short line) {
	dbuff->Write_Buffer({ score_box_xy.X + 2, score_box_xy.Y - line }, str);
}

void Screen::Print_Map_Boundary(DoubleBuffering* dbuff, Box box) {
	for (short i = 0; i < box.size.X * 2 - 1; i++) {
		dbuff->Write_Buffer({ i + box.xy.X, box.xy.Y }, WALL_ANSI_COLOR_CYAN);
	}
	for (short i = 1; i < box.size.Y; i++) {
		dbuff->Write_Buffer({ box.xy.X, i + box.xy.Y }, WALL_ANSI_COLOR_CYAN);
	}
	for (short i = 0; i < box.size.X * 2 - 1; i++) {
		dbuff->Write_Buffer({ i + box.xy.X, box.size.Y + box.xy.Y }, WALL_ANSI_COLOR_CYAN);
	}
	for (short i = 1; i < box.size.Y; i++) {
		dbuff->Write_Buffer({ (box.size.X * 2 - 2 + box.xy.X), i + box.xy.Y }, WALL_ANSI_COLOR_CYAN);
	}
}

void Screen::Print_Score_Board(DoubleBuffering* dbuff, Score_Box score_box) {
	for (short i = 0; i < score_box.size.X * 2 - 1; i++) {
		dbuff->Write_Buffer({ i + score_box.xy.X, score_box.xy.Y }, WALL);
	}
	for (short i = 1; i < score_box.size.Y; i++) {
		dbuff->Write_Buffer({ score_box.xy.X, i + score_box.xy.Y }, WALL);
	}
	for (short i = 0; i < score_box.size.X * 2 - 1; i++) {
		dbuff->Write_Buffer({ i + score_box.xy.X, score_box.size.Y + score_box.xy.Y }, WALL);
	}
	for (short i = 1; i < score_box.size.Y; i++) {
		dbuff->Write_Buffer({ (score_box.size.X * 2 - 2 + score_box.xy.X), i + score_box.xy.Y }, WALL);
	}
}

void Object::Print_Reset_Ball(DoubleBuffering* dbuff, Keyboard key, Ball* ball) {
	dbuff->Write_Buffer({ ball->xy.X, ball->xy.Y }, BALL);
	key.Player_Ball_Input(ball); //SpaceŰ�� �Է¹����� ball.fall_down�� FALSE�� �ٲٰ� �⺻ ���ñ��� �Ϸ���.
}

string Object::repeat_str(string s, int n) {
	string s1 = s;
	for (int i = 0; i < n; i++) {
		s += s1;
	}
	return s;
}

char* Object::color_set(char* color) { //�����ڵ� 1�� �����ְ� ���������� 1�� Back
	int n = static_cast<int>(color[6]);

	if (n == 6)
		n = 1;
	else
		n++;

	color[6] = static_cast<char>(n);
	return color;
}
void Object::Print_Player(DoubleBuffering* dbuff, Player *player, Keyboard key, Box box) {

	key.Player_X_Input(&(player->xy.X));

	if ((player->xy.X + player->length) >= box.size.X * 2 + box.xy.X) {
		player->xy.X -= 2;
	}
	else if ((player->xy.X) <= 1 + box.xy.X) {
		player->xy.X += 2;
	}

	for (short i = 0; i < player->length; i++) {
		dbuff->Write_Buffer({ player->xy.X + i, player->xy.Y }, PLAYER);
	}
}
void Object::Print_Wall(DoubleBuffering* dbuff, Wall* wall, SWall* swall, Box box) {
	char* default_color = DESTROYABLE_WALL;

	for (short i = 0; i < wall->nblocks; i++) {
		//default_color = color_set(default_color);
		if (swall[i].is_crashed == FALSE) {
			dbuff->Write_Buffer({ swall[i].xy.X, swall[i].xy.Y }, repeat_str(default_color, swall[i].length));
		}
	}

	/////////////////////////////////////////////////////////
	std::string debug5 = std::to_string(wall->nblocks);
	dbuff->Write_Buffer({ 150, 34 }, debug5);
	////////////////////////////////////////////////////////
}

void Object::Crash_Wall(Ball* ball, Wall* wall, SWall* swall) {
	BallMovement ball_move;
	for (short i = 0; i < wall->nblocks; i++) {
		if (swall[i].is_crashed == FALSE) {
			if (ball->xy.X <= (swall[i].xy.X + swall[i].length) && ball->xy.X >= swall[i].xy.X && swall[i].xy.Y <= ball->xy.Y && swall[i].xy.Y >= ball->xy.Y) { //if crashed
				if ((swall[i].xy.X + wall->block_length == swall[i + 1].xy.X) || (swall[i].xy.X - wall->block_length == swall[i - 1].xy.X)) { //Horizontal Collision Resolution
					swall[i].is_crashed = TRUE;
					ball->upbound = TRUE;
					ball_move.vec_direction(ball);
				}
				else { //Vertical Collision Resolution
					swall[i].is_crashed = TRUE;
					ball->upbound = FALSE;
					ball_move.vec_direction(ball);
				}
				/*size_t array_size = wall->nblocks;
				memmove(swall + i, swall + i + 1, (array_size - i - 1) * sizeof(SWall));
				wall->nblocks--;*/
			}
		}
	}
}

SWall* Object::Config_Wall(Wall* wall, Box box) {
	/*
	wall->nblocks = (((box.size.X * 2) / wall->block_length)) * (wall->height);
	wall->width = (((box.size.X * 2) / wall->block_length));

	char** wall_set = new char*[wall->width];

	for (int i = 0; i < wall->height; i++) {
		wall_set[i] = new char[wall->height];
	}

	wall_set = {};

	SWall* swall = new SWall[wall->nblocks]();
	*/
	/*
	for (short i = 0; i < wall->width; i++) {
		for (short j = 0; i < wall->height; k++) {
			swall[num_blocks].xy = { (box.xy.X - 3) + wall->block_length, i + wall->y };
			swall[num_blocks].length = wall->block_length;
			num_blocks++;
		}
	}*/

	
	wall->nblocks = ((box.size.X * 2) / wall->block_length) * (wall->height);
	SWall* swall = new SWall[wall->nblocks](); //delete �κ�

	int l = 0;
	short tmp_block_size;
	for (short i = wall->y; i < wall->height + wall->y; i++) {
		for (short j = box.xy.X + 2; j < ((box.size.X * 2) + (box.xy.X + 2)); j += wall->block_length) {

			if (j+wall->block_length > ((box.size.X * 2 - 5) + (box.xy.X + 2))) {
				tmp_block_size = ((box.size.X * 2 - 5) % wall->block_length);
				cout << tmp_block_size << endl;
				swall[l].xy = { j, i };
				swall[l].length = tmp_block_size;
				l++;
			}
			else{
				swall[l].xy = { j, i };
				swall[l].length = wall->block_length;
				l++;
			}
		}
	}
	
	return swall;
}
void Object::Print_Ball(DoubleBuffering* dbuff, Ball *ball, Box box, Player player, Keyboard key) {

	if (ball->fall_down) {
		ball->xy = { (player.xy.X + (player.length / 2)), player.xy.Y - 1 };
		Print_Reset_Ball(dbuff, key, ball);

	}
	else if (!ball->fall_down) {
		BallMovement ball_move;

		if ((ball->xy.X) >= box.size.X * 2 + box.xy.X - 3) { //�����ʺ�
			ball->xy.X -= ball->speed;
			ball->upbound = FALSE;
			ball_move.vec_direction(ball, TRUE);
		}

		else if (ball->xy.X <= box.xy.X + 1) { //���ʺ�
			ball->xy.X += ball->speed;
			ball->upbound = FALSE;
			ball_move.vec_direction(ball);
		}

		if (ball->xy.Y <= box.xy.Y + 1) { //���� ���� �ε�ġ��
			ball->xy.Y += ball->speed;
			ball->upbound = TRUE;
			ball_move.vec_direction(ball);
		}
		/*
		if (ball->xy.Y >= player.xy.Y - 1 && ball->xy.X <= (player.xy.X + player.length) && ball->xy.X >= (player.xy.X)) {
			ball->xy.Y -= ball->speed;
			ball->upbound = TRUE;
			ball_move.vec_direction(ball);
			Beep(523, 50);
		}*/
		if (ball->xy.X <= (player.xy.X + player.length) && ball->xy.X >= (player.xy.X)) {
			if (ball->xy.Y >= player.xy.Y - 1 && !(ball->xy.Y >= player.xy.Y + 1)) {
				ball->xy.Y -= ball->speed;
				ball->upbound = TRUE;
				ball_move.vec_direction(ball);
				Beep(523, 50);
			}
		}

		if ((ball->xy.Y) >= box.size.Y + box.xy.Y) { //�Ʒ��� ���� 
			ball->xy.Y -= ball->speed;
			ball->fall_down = TRUE;
			//ptr_direction = ball_move.vec_direction(ball->direction, ball->upbound, FALSE);
			//ball_move.find_direction(ball, ptr_direction);
		}

		ball->xy.X = (ball->xy.X + (ball->direction_xy.X * ball->speed));
		ball->xy.Y = (ball->xy.Y + (ball->direction_xy.Y * ball->speed));

		//////////////////////////////////////////////////////////////////////////
		std::string debug1 = std::to_string(ball->xy.X) + ", " + std::to_string(ball->xy.Y);
		std::string debug2 = std::to_string(ball->direction[0]) + std::to_string(ball->direction[1]) + "," + std::to_string(ball->direction[2]) + std::to_string(ball->direction[3]);
		std::string debug3 = std::to_string(ball->direction_xy.X) + std::to_string(ball->direction_xy.Y);
		std::string debug4 = std::to_string(ball->upbound);
		dbuff->Write_Buffer({ 150, 30 }, debug1);
		dbuff->Write_Buffer({ 150, 31 }, debug2);
		dbuff->Write_Buffer({ 150, 32 }, debug3);
		dbuff->Write_Buffer({ 150, 33 }, debug4);
		/////////////////////////////////////////////////////////////////////////

		dbuff->Write_Buffer({ ball->xy.X, ball->xy.Y }, BALL);
	}
}

void Main_Screen::Print(DoubleBuffering *dbuff, Box box, Score_Box score_box) {
	Print_Map_Boundary(dbuff, box);
	Print_Score_Board(dbuff, score_box);
}