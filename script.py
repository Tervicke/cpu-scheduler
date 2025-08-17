import json
import matplotlib.pyplot as plt
from matplotlib.colors import TABLEAU_COLORS
from pathlib import Path

# Load JSON from cmake-build-debug
json_file = Path(__file__).parent / "cmake-build-debug/gantt.json"
with open(json_file) as f:
    data = json.load(f)

num_processes = len(data["plot"])
fig_height = max(4, num_processes * 0.8)
fig, gs = plt.subplots(figsize=(8, fig_height), nrows=2, gridspec_kw={'height_ratios':[3, 1]})

# Add a main title for the algorithm
algorithm_name = data.get("algorithm", "Scheduling Algorithm")
fig.suptitle(f"{algorithm_name}", fontsize=16, fontweight='bold')

# Split axes
ax_gantt, ax_table = gs

# Plot Gantt chart
colors = list(TABLEAU_COLORS.values())
for i, p in enumerate(data["plot"]):
    color = colors[i % len(colors)]
    ax_gantt.barh(p["id"], p["end"] - p["start"], left=p["start"],
                  color=color, edgecolor='black', height=0.5)
    ax_gantt.text(p["start"], i, str(p["start"]), va='center', ha='right', fontsize=8)
    ax_gantt.text(p["end"], i, str(p["end"]), va='center', ha='left', fontsize=8)

ax_gantt.set_xlabel("Time")
ax_gantt.set_title("Gantt Chart", fontsize=14, fontweight='bold')
ax_gantt.grid(axis='x', linestyle='--', alpha=0.5)

# Prepare table data
table_rows = [
    [p["id"], p["AT"], p["BT"], p["CT"], p["TAT"], p["WT"]] 
    for p in data["processes"]
]

# Calculate averages
avg_TAT = sum(p["TAT"] for p in data["processes"]) / num_processes
avg_WT  = sum(p["WT"] for p in data["processes"]) / num_processes

# Append average row
table_rows.append(["Avg", "", "", "", f"{avg_TAT:.2f}", f"{avg_WT:.2f}"])
columns = ["ID", "AT", "BT", "CT", "TAT", "WT"]

# Plot table
ax_table.axis('off')
table = ax_table.table(cellText=table_rows, colLabels=columns, loc='center',
                       cellLoc='center', edges='horizontal')
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1, 1.3)

# Alternating row colors, skip the last avg row
for i, _ in enumerate(table_rows[:-1]):
    if i % 2 == 0:
        for j in range(len(columns)):
            table[(i+1, j)].set_facecolor("#f2f2f2")

# Highlight average row
for j in range(len(columns)):
    table[(len(table_rows), j)].set_facecolor("#d1e7dd")  # light green
    table[(len(table_rows), j)].set_text_props(weight='bold')

plt.tight_layout(rect=[0, 0, 1, 0.95])  # leave space for suptitle
plt.show()

