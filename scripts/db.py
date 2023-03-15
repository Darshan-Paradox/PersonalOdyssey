import sys
import sqlite3 as sq

tasks = sq.connect("./db/tasks.db")
stats = sq.connect("./db/stats.db")

curs = tasks.cursor()
cur = stats.cursor()

if __name__ == "__main__":
    args = sys.argv

    if args[1] == "tasks":
        curs.execute(f"SELECT * FROM tasks")
        query = curs.fetchall()

        if len(query) - 1 < int(args[2]):
            print("")
        else:
            print(query[int(args[2])][1])

    if args[1] == "status":
        curs.execute(f"SELECT * FROM tasks")
        query = curs.fetchall()

        if len(query) - 1 < int(args[2]):
            print("\n")
        else:
            print(query[int(args[2])][3])

    if args[1] == "set_status":
        curs.execute(f"SELECT rowid,* FROM tasks")
        curs.execute(f"""UPDATE tasks
                     SET done = {int(args[3])}
                     WHERE rowid = {int(args[2]) + 1}""")

    if args[1] == "stats":
        cur.execute(f"SELECT * FROM stats WHERE prop = '{args[2]}'")
        query = cur.fetchall()

        print(f"{query[0][-1] :1f}")

    if args[1] == "set_stats":
        cur.execute(f"UPDATE stats SET val = {args[3]} WHERE prop = '{args[2]}'")

stats.commit()
tasks.commit()

tasks.close()
stats.close()
