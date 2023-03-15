void get_task(char *task, int size, int task_no)
{
	char query[128];
	sprintf(query, "python3 scripts/db.py tasks %d", task_no);

	FILE *fp = popen(query, "r");
	fgets(task, size, fp);

	pclose(fp);
}

int get_surprise(char *task, int size)
{
	char query[128];
	sprintf(query, "python3 scripts/db.py tasks 5");

	FILE *fp = popen(query, "r");
	fgets(task, size, fp);
	pclose(fp);

	if (strcmp(task, "\n") == 0)
	{
		strcpy(task, "No surprise task today");
		return 0;
	}

	return 1;
}

int get_completed(int task_no)
{
	char query[128];
	sprintf(query, "python3 scripts/db.py status %d", task_no);

	FILE *fp = popen(query, "r");
	char c =fgetc(fp);

	pclose(fp);

	if (c == '0')
		return 0;
	else if (c == '1')
		return 1;
	else
		return -1;
}

void set_completed(int done, int task_no)
{
	char query[128];
	sprintf(query, "python3 scripts/db.py set_status %d %d", task_no, done);

	FILE *fp = popen(query, "r");

	pclose(fp);
}

double get_stats(char *stat)
{
	char query[128], result[128];
	sprintf(query, "python3 scripts/db.py stats %s", stat);

	FILE *fp = popen(query, "r");
	fgets(result, 128, fp);
	pclose(fp);

	return atof(result);
}

void set_stats(char *stat, double val)
{
	char query[128];
	sprintf(query, "python3 scripts/db.py set_stats %s %.1f", stat, val);

	FILE *fp = popen(query, "r");
	pclose(fp);
}
