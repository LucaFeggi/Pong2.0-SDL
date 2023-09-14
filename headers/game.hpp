class game{
	public:
		game();
		~game();
		void mover();
		void control_window_resize();
		bool has_scored(paddle *left_paddle, paddle *right_paddle);
		void restart();
		bool is_over();
		void draw_score();
		void draw_game_over();
		void draw_pause();
		void erase_pause();
		void cycle(bool is_paused);
	private:
		ball *mball;
		paddle *left_paddle;
		paddle *right_paddle;
		sound *msound;
		SDL_Surface *left_text = NULL;
		SDL_Surface *right_text = NULL;
		SDL_Surface *over_text = NULL;
		SDL_Surface *pause_text = NULL;
		const uint8_t *state = SDL_GetKeyboardState(NULL);
		unsigned short left_score;
		unsigned short right_score;
};

game::game(){
	mball = new ball();
	left_paddle = new paddle(window_width / 12.5f);
	right_paddle = new paddle(window_width - window_width / 12.5f);
	msound = new sound();
	left_score = 0;
	right_score = 0;
}

game::~game(){
	delete mball;
	delete left_paddle;
	delete right_paddle;
	delete msound;
}

void game::mover(){
	if(state[SDL_SCANCODE_LSHIFT])
		if(left_paddle->get_y() - paddle_height / 2.0f > global_velocity)
			left_paddle->move_up();
	if(state[SDL_SCANCODE_LCTRL])
		if(left_paddle->get_y() + paddle_height / 2.0f < window_height - global_velocity)
			left_paddle->move_down();
	if(state[SDL_SCANCODE_UP])
		if(right_paddle->get_y() - paddle_height / 2.0f > global_velocity)
			right_paddle->move_up();
	if(state[SDL_SCANCODE_DOWN])
		if(right_paddle->get_y() + paddle_height / 2.0f < window_height - global_velocity)
			right_paddle->move_down();
}

void game::control_window_resize(){
	int new_width, new_height;
	SDL_GetWindowSize(window, &new_width, &new_height);
	if(new_width != window_width || new_height != window_height){
		vec2 new_ball_pos((mball->get_x() * new_width) / window_width, (mball->get_y() * new_height) / window_height);
 		vec2 new_left_paddle_pos((left_paddle->get_x() * new_width) / window_width, (left_paddle->get_y() * new_height) / window_height);
 		vec2 new_right_paddle_pos((right_paddle->get_x() * new_width) / window_width, (right_paddle->get_y() * new_height) / window_height);
 		vec2 new_vel((mball->get_vel().get_x() * new_width) / window_width, (mball->get_vel().get_y() * new_height) / window_height);
 		delete mball;
 		delete left_paddle;
 		delete right_paddle;
		window_width = new_width;
		window_height = new_height;
		SDL_FreeSurface(surface);
		surface = SDL_GetWindowSurface(window);
		font_size = window_width / 50.0f;
		paddle_width = window_width / 100.0f;
		paddle_height = window_height / 7.5f;
		ball_radius = paddle_width / 2.0f;
		global_velocity = window_width / 240.0f;
		TTF_CloseFont(font);
		font = TTF_OpenFont("font/pixelated.ttf", (int)(font_size * 2.0f));
		mball = new ball(new_ball_pos.get_x(), new_ball_pos.get_y(), new_vel);
		left_paddle = new paddle(new_left_paddle_pos.get_x(), new_left_paddle_pos.get_y());
		right_paddle = new paddle(new_right_paddle_pos.get_x(), new_right_paddle_pos.get_y());
	}
}

bool game::has_scored(paddle *left_paddle, paddle *right_paddle){
	if(mball->get_x() < left_paddle->get_x() - 6.0f * paddle_width)
		right_score++;
	else if(mball->get_x() > right_paddle->get_x() + 6.0f * paddle_width)
		left_score++;
	else
		return false;
	return true;
}

void game::restart(){
	delete mball;
	delete left_paddle;
	delete right_paddle;
	mball = new ball();
	left_paddle = new paddle(window_width / 12.5f);
	right_paddle = new paddle(window_width - window_width / 12.5f);
}

bool game::is_over(){
	if(left_score == 11 || right_score == 11)
		return true;
	return false;
}

void game::draw_score(){
	
	std::stringstream l_stream;
	l_stream << left_score;
	std::string left_str = l_stream.str();
	left_text = TTF_RenderText_Shaded(font, left_str.c_str(), white, black);
	SDL_Rect l_rect = {(surface -> w - left_text -> w) / 4, 0, 0, 0};
	SDL_BlitSurface(left_text, NULL, surface, &l_rect);
	SDL_FreeSurface(left_text);

	std::stringstream r_stream;
	r_stream << right_score;
	std::string right_str = r_stream.str();
	right_text = TTF_RenderText_Shaded(font, right_str.c_str(), white, black);
	SDL_Rect r_rect = {(surface -> w - right_text -> w) / 4 * 3, 0, 0, 0};
	SDL_BlitSurface(right_text, NULL, surface, &r_rect);
	SDL_FreeSurface(right_text);
	
}

void game::draw_game_over(){
	std::string over_str;
	left_score == 11 ? over_str = "left player won" : over_str = "right player won";
	over_text = TTF_RenderText_Shaded(font, over_str.c_str(), white, black);
	SDL_Rect over_rect = {(surface -> w - over_text -> w) / 2, (surface -> h - over_text -> h) / 2, 0, 0};
	SDL_BlitSurface(over_text, NULL, surface, &over_rect);
	SDL_FreeSurface(over_text);
}

void game::draw_pause(){
	std::string pause_str = "pause";
	pause_text = TTF_RenderText_Shaded(font, pause_str.c_str(), white, black);
	SDL_Rect pause_rect = {(surface -> w - pause_text -> w) / 2, (surface -> h - pause_text -> h) / 2, 0, 0};
	SDL_BlitSurface(pause_text, NULL, surface, &pause_rect);
	SDL_FreeSurface(pause_text);
}

void game::erase_pause(){
	std::string pause_str = "pause";
	pause_text = TTF_RenderText_Shaded(font, pause_str.c_str(), black, black);
	SDL_Rect pause_rect = {(surface -> w - pause_text -> w) / 2, (surface -> h - pause_text -> h) / 2, 0, 0};
	SDL_BlitSurface(pause_text, NULL, surface, &pause_rect);
	SDL_FreeSurface(pause_text);
}

void game::cycle(bool is_paused){
	double IterationStart = SDL_GetPerformanceCounter();
	this->control_window_resize();
	mball->erase();
	left_paddle->erase();
	right_paddle->erase();
 	if(!is_paused){
		this->erase_pause();
		if(!this->is_over()){
			this->mover();
			if(mball->is_wall_colliding()){
				mball->update_vel_wall();
				msound->play_wall();
			}
			if(mball->is_paddle_colliding(left_paddle, right_paddle)){
				mball->update_vel_paddle(left_paddle, right_paddle);
				msound->play_paddle();
			}
			mball->update_pos();
			if(this->has_scored(left_paddle, right_paddle)){
				this->restart();
				msound->play_score();
			}
		}
		else
				this->draw_game_over();
 	}
 	else
		if(!this->is_over())
			this->draw_pause();
	mball->draw();
	left_paddle->draw();
	right_paddle->draw();
	this->draw_score();
	SDL_UpdateWindowSurface(window);
	double IterationEnd = SDL_GetPerformanceCounter();
	double ElapsedSeconds = (IterationEnd - IterationStart) / (double)SDL_GetPerformanceFrequency();
	double Delay = 11.111 - (ElapsedSeconds * 1000.0);
	if(Delay > 0)
		SDL_Delay(std::max(0, (int) Delay));
}
