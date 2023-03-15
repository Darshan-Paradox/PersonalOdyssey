struct Window
{
	WINDOW *win;
	int height;
	int width;
	int y0;
	int x0;
};

typedef struct Window Window;

Window *create_win(int height, int width, int y0, int x0)
{
	Window *win = (Window *)malloc(sizeof(Window));

	win->height = height;
	win->width = width;
	win->y0 = y0;
	win->x0 = x0;

	win->win = newwin(height, width, y0, x0);

	return win;
}

void title(Window *win, const char *title, int row, int col)
{
	attron(A_BOLD | A_UNDERLINE | COLOR_PAIR(1));

	if (win == NULL)
		mvwprintw(stdscr, 1, (col - strlen(title))/2, "%s", title);
	else
		mvwprintw(win->win, 1, (col - strlen(title))/2, "%s", title);
	attroff(A_BOLD | A_UNDERLINE | COLOR_PAIR(1));
	refresh();

}

int is_in(Window *win, int y, int x)
{
	if ((x > win->x0) && (x < (win->x0 + win->width)))
		if ((y > win->y0) && (y < (win->y0 + win->height)))
			return 1;

	return 0;
}
