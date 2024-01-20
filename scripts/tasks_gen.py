import numpy as np
import sqlite3 as sq
from datetime import datetime

coding = []
academics = []
physical = []
surprise = []

dt = datetime.now()

iday = dt.weekday()

conn = sq.connect("./db/stats.db")
cur = conn.cursor()

cur.execute("SELECT * FROM stats WHERE prop = 'level'")
level = cur.fetchall()[0][1]

conn.commit()
conn.close()

## physical task logic
if iday == 1:
    physical.append(f"Do cardio-vascular exercises + yoga exercises")
elif iday == 2:
    physical.append(f"Lower Body:: Squats, Deadlifts, Hip Thrusts, Lunges")
elif iday == 3:
    physical.append(f"Upper Body and Core:: Bicep curl, Tricep dip, Chest press, Core circuit")
elif iday == 4:
    physical.append(f"Do yoga exercises for flexibility")
elif iday == 5:
    physical.append(f"Lower Body:: Squats, Glute bridges, Hip Thrusts, Calf extension")
elif iday == 6:
    physical.append(f"Upper Body:: Shoulder press, Lateral raise, Reverse fly, Seated rows, Lat pulls")
elif iday == 0:
    physical.append(f"Do meditation  + yoga exercises")

## academic task logic
academics_lower = np.abs(level)
academics_upper = academics_lower + 3
if iday == 1:
    course = "Analog Electronics"
elif iday == 3:
    course = "Power Electronics"
elif iday == 4:
    course = "Industrial Instrumentation and Control"
elif iday == 5:
    course = "Mathematical Methods of Physics"
elif iday == 6:
    course = "Electro-magnetic Theory and Quantum Mechanics"
elif iday == 0:
    course = "Analog Electronics"
elif iday == 2:
    course = "Power Electronics"

academics.append(f"Solve {np.random.randint(academics_lower + 2, academics_upper)} questions of {course}")
if iday % 2 == 0:
    _course = "Complete one chapter of Business Analytics"
else:
    _course = "Learn about one Machine Learning algorithm and implement it on a dataset"
academics.append(f"{_course}")

## Coding task logic
leet_lower = np.abs(level)
leet_upper = leet_lower + 2

coding.append(f"Solve {np.random.randint(leet_lower, leet_upper)} questions from Leetcode (Easy)")
coding.append(f"Solve {np.random.randint(leet_lower, leet_upper)} questions from Leetcode (Medium)")
coding.append(f"Solve {np.random.randint(leet_lower, leet_upper)} questions from CSES")
coding.append(f"Implement a Data Structure or Algorithms")

## surprise tasks
activity = ["pushups", "crunches", "squats"]

surprise.append(f"Run {np.random.randint(1, np.abs(level) + 1)}km")
surprise.append(f"Do {5*np.random.randint(1, np.abs(level) + 1)} sets of {activity[np.random.randint(len(activity))]}")

surprise.append(f"Solve https://leetcode.com/problems/random-one-question/all {np.ceil(1.5*leet_lower)} times")
surprise.append(f"Solve {np.random.randint(academics_upper, academics_upper + 5)} questions of {course}")

def get_random_tasks():
    tasks = []

    tasks.append(coding[np.random.randint(len(coding))])
    tasks.append(coding[np.random.randint(len(coding))])

    tasks.append(academics[0])
    tasks.append(academics[1])

    tasks.append(physical[0])

    if np.random.rand() < 0.4:
        tasks.append(surprise[np.random.randint(len(surprise))])

    return tasks
