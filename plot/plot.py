"""
Generate a Gantt chart of events on our RTOS vs time.
"""

import re
import matplotlib.pyplot as plt

def plot_rtos_log(filename, title="RTOS Gantt Chart"):
    # --- Step 1: Parse the log for events ---
    events = []

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            tim2_interrupt = re.search(r'Tim2 running\. Time: (\d+)', line)
            tim3_interrupt = re.search(r'Tim3 running\. Time: (\d+)', line)
            start_long = re.match(r'Long task started\. Time: (\d+)', line)
            pause_long = re.match(r'Long task paused\. Time: (\d+)', line)
            resume_long = re.match(r'Long task resumed\. Time: (\d+)', line)
            finish_long = re.match(r'Long task finished\. Time: (\d+)', line)
            start_short = re.match(r'Short task started\. Time: (\d+)', line)
            pause_short = re.match(r'Short task paused\. Time: (\d+)', line)
            resume_short = re.match(r'Short task resumed\. Time: (\d+)', line)
            finish_short = re.match(r'Short task finished\. Time: (\d+)', line)
            start_dep = re.match(r'Dependent task started. Time: (\d+)', line)
            pause_dep = re.match(r'Dependent task paused\. Time: (\d+)', line)
            resume_dep = re.match(r'Dependent task resumed\. Time: (\d+)', line)
            finish_dep = re.match(r'Dependent task finished. Time: (\d+)', line)

            if tim2_interrupt:
                events.append({'task': 'TIM2', 'action': 'temp_interrupt', 'time': int(tim2_interrupt.group(1))})
            elif tim3_interrupt:
                events.append({'task': 'TIM3', 'action': 'temp_interrupt', 'time': int(tim3_interrupt.group(1))})
            elif start_long:
                events.append({'task': 'Long Task', 'action': 'start', 'time': int(start_long.group(1))})
            elif pause_long:
                events.append({'task': 'Long Task', 'action': 'end', 'time': int(pause_long.group(1))})
            elif resume_long:
                events.append({'task': 'Long Task', 'action': 'start', 'time': int(resume_long.group(1))})
            elif finish_long:
                events.append({'task': 'Long Task', 'action': 'end', 'time': int(finish_long.group(1))})
            elif start_short:
                events.append({'task': 'Short Task', 'action': 'start', 'time': int(start_short.group(1))})
            elif pause_short:
                events.append({'task': 'Short Task', 'action': 'end', 'time': int(pause_short.group(1))})
            elif resume_short:
                events.append({'task': 'Short Task', 'action': 'start', 'time': int(resume_short.group(1))})
            elif finish_short:
                events.append({'task': 'Short Task', 'action': 'end', 'time': int(finish_short.group(1))})
            elif start_dep:
                events.append({'task': 'Dependent Task', 'action': 'start', 'time': int(start_dep.group(1))})
            elif pause_dep:
                events.append({'task': 'Dependent Task', 'action': 'end', 'time': int(pause_dep.group(1))})
            elif resume_dep:
                events.append({'task': 'Dependent Task', 'action': 'start', 'time': int(resume_dep.group(1))})
            elif finish_dep:
                events.append({'task': 'Dependent Task', 'action': 'end', 'time': int(finish_dep.group(1))})

    # --- Step 2: Group into task durations ---
    task_times = {'Long Task': [], 'Short Task': [], 'Dependent Task': []}
    running_tasks = {}
    single_events = {'TIM3': [], 'TIM2': []}

    for event in events:
        task = event['task']
        action = event['action']
        time = event['time']

        if action == 'start':
            running_tasks[task] = time
        elif action == 'end':
            if task in running_tasks:
                start_time = running_tasks.pop(task)
                task_times[task].append((start_time, time))
        elif action == 'temp_interrupt':
            single_events[task].append(time)

    # --- Step 3: Plot using matplotlib ---
    fig, ax = plt.subplots(figsize=(10, 5))

    colors = {'Long Task': 'skyblue', 'Short Task': 'lightcoral', 'Dependent Task': 'purple'}
    y_pos = {'Long Task': 0, 'Short Task': 1, 'Dependent Task': 2}

    for task, periods in task_times.items():
        for start, end in periods:
            ax.barh(y_pos[task], end - start, left=start, color=colors[task], edgecolor='black', label=task)

    for event_name, times in single_events.items():
        dot_size = 100 if event_name == 'TIM2' else 10
        label = f'{event_name} Interrupt'
        ax.scatter(times, [2.5] * len(times), label=label, marker='.', edgecolor='black', facecolor='none', s=dot_size)

    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(by_label.values(), by_label.keys())

    ax.set_yticks([y_pos['Long Task'], y_pos['Short Task'], y_pos['Dependent Task']])
    ax.set_yticklabels(['Long Task', 'Short Task', 'Dependent Task'])
    ax.set_xlabel('Time (ticks)')
    ax.set_title(title)
    plt.tight_layout()
    plt.show()
