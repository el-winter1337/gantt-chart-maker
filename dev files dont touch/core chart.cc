<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gantt Chart Maker</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap');

        :root {
            --bg-dark: #0f172a;
            --card-bg: rgba(30, 41, 59, 0.75);
            --text-main: #f8fafc;
            --text-dim: #94a3b8;
            --primary-blue: #3b82f6;
            --primary-green: #10b981;
            --primary-orange: #f59e0b;
            --primary-pink: #ec4899;
            --primary-purple: #8b5cf6;
            --border-color: rgba(255, 255, 255, 0.1);
        }

        *,
        *::before,
        *::after {
            box-sizing: border-box;
        }

        /* Custom Scrollbars */
        ::-webkit-scrollbar {
            width: 6px;
            height: 8px;
        }

        ::-webkit-scrollbar-track {
            background: transparent;
        }

        ::-webkit-scrollbar-thumb {
            background: rgba(255, 255, 255, 0.2);
            border-radius: 10px;
        }

        ::-webkit-scrollbar-thumb:hover {
            background: rgba(255, 255, 255, 0.3);
        }

        body {
            font-family: 'Inter', -apple-system, sans-serif;
            background: linear-gradient(135deg, #020617 0%, #172554 100%);
            color: var(--text-main);
            display: flex;
            justify-content: center;
            padding: 40px 20px;
            margin: 0;
            min-height: 100vh;
            box-sizing: border-box;
            overflow-x: hidden;
        }

        .simulator-card {
            background: var(--card-bg);
            backdrop-filter: blur(24px);
            -webkit-backdrop-filter: blur(24px);
            width: 1400px;
            max-width: 100%;
            padding: 30px 40px;
            border-radius: 20px;
            box-shadow: 0 40px 80px -20px rgba(0, 0, 0, 0.8), inset 0 1px 0 rgba(255, 255, 255, 0.1);
            display: grid;
            grid-template-columns: 350px 1fr;
            gap: 40px;
            border: 1px solid rgba(255, 255, 255, 0.08);
        }

        /* Responsive */
        @media (max-width: 1024px) {
            .simulator-card {
                grid-template-columns: 1fr;
            }
        }

        /* Common Editable Box Style */
        .editable-box {
            background: rgba(0, 0, 0, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.08);
            border-radius: 6px;
            padding: 4px 8px;
            outline: none;
            transition: all 0.2s;
            cursor: text;
            display: inline-block;
            color: inherit;
            font-family: inherit;
        }

        .editable-box:hover {
            background: rgba(0, 0, 0, 0.4);
            border-color: rgba(255, 255, 255, 0.3);
        }

        .editable-box:focus {
            border-color: var(--primary-blue);
            background: rgba(0, 0, 0, 0.5);
            box-shadow: 0 0 0 2px rgba(59, 130, 246, 0.3);
        }

        /* Title Area */
        .title-area h1 {
            font-size: 28px;
            margin: 0;
            font-weight: 700;
            letter-spacing: -0.5px;
            background: linear-gradient(90deg, #fff, #93c5fd);
            -webkit-background-clip: text;
            background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        /* Top Header Stats */
        .header {
            grid-column: 1 / -1;
            display: flex;
            justify-content: space-between;
            align-items: flex-end;
            margin-bottom: 10px;
            padding-bottom: 20px;
            border-bottom: 1px solid var(--border-color);
            flex-wrap: wrap;
            gap: 20px;
        }

        .stats-area {
            display: flex;
            gap: 30px;
        }

        .stat-box {
            text-align: left;
        }

        .stat-label {
            font-size: 11px;
            text-transform: uppercase;
            color: var(--text-dim);
            letter-spacing: 1px;
            font-weight: 600;
            margin-bottom: 4px;
        }

        .stat-value {
            font-size: 20px;
            font-weight: 700;
            display: flex;
            align-items: center;
            gap: 5px;
        }

        /* Left Side: Controls */
        .controls-panel {
            display: flex;
            flex-direction: column;
            gap: 20px;
        }

        .form-group {
            display: flex;
            flex-direction: column;
            gap: 8px;
            align-items: flex-start;
        }

        .form-group label {
            font-size: 12px;
            color: var(--text-dim);
            font-weight: 600;
        }

        .form-group input,
        .form-group select {
            background: rgba(0, 0, 0, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.05);
            color: white;
            padding: 12px;
            border-radius: 10px;
            font-family: inherit;
            font-size: 14px;
            transition: all 0.2s;
            width: 100%;
        }

        .form-group input:focus,
        .form-group select:focus {
            outline: none;
            border-color: var(--primary-blue);
            background: rgba(0, 0, 0, 0.4);
        }

        .btn {
            background: var(--primary-blue);
            background: linear-gradient(135deg, #3b82f6 0%, #2563eb 100%);
            color: white;
            border: none;
            padding: 14px;
            border-radius: 10px;
            cursor: pointer;
            font-weight: 600;
            font-size: 14px;
            transition: all 0.2s;
            box-shadow: 0 4px 15px rgba(59, 130, 246, 0.4);
            width: 100%;
        }

        .btn:hover {
            transform: translateY(-1px);
            box-shadow: 0 6px 20px rgba(59, 130, 246, 0.5);
        }

        .task-list {
            margin-top: 10px;
            display: flex;
            flex-direction: column;
            gap: 12px;
            max-height: 400px;
            overflow-y: auto;
            padding-right: 5px;
        }

        .task-item {
            background: rgba(255, 255, 255, 0.03);
            border: 1px solid rgba(255, 255, 255, 0.05);
            padding: 12px;
            border-radius: 12px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            transition: all 0.2s;
        }

        .task-item:hover {
            background: rgba(255, 255, 255, 0.06);
            border-color: rgba(255, 255, 255, 0.1);
        }

        .task-item-delete {
            background: rgba(239, 68, 68, 0.1);
            color: #ef4444;
            border: 1px solid transparent;
            border-radius: 8px;
            width: 28px;
            height: 28px;
            cursor: pointer;
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 16px;
            transition: all 0.2s;
        }

        .task-item-delete:hover {
            background: #ef4444;
            color: white;
        }

        /* Inline Input matches editable-box mapping nicely */
        .inline-input {
            background: rgba(0, 0, 0, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.08);
            color: inherit;
            font-family: inherit;
            outline: none;
            padding: 4px 6px;
            border-radius: 6px;
            transition: all 0.2s;
        }

        .inline-input:hover {
            background: rgba(0, 0, 0, 0.3);
            border-color: rgba(255, 255, 255, 0.2);
        }

        .inline-input:focus {
            border-color: var(--primary-blue);
            background: rgba(0, 0, 0, 0.4);
            box-shadow: 0 0 0 1px rgba(59, 130, 246, 0.3);
        }

        /* Right Side: Chart Area */
        .chart-panel {
            display: flex;
            flex-direction: column;
        }

        .chart-container {
            position: relative;
            height: 500px;
            margin-top: 10px;
            border-bottom: 1px solid var(--border-color);
            overflow-x: auto;
            overflow-y: auto;
            width: 100%;
        }

        .chart-inner {
            position: relative;
            min-width: 100%;
            height: 100%;
            min-height: 480px;
        }

        .grid-line {
            position: absolute;
            top: 0;
            bottom: 0;
            width: 1px;
            border-left: 1px dashed rgba(255, 255, 255, 0.1);
            z-index: 1;
        }

        /* Task Bars */
        .task-row {
            position: absolute;
            width: 100%;
            height: 44px;
            display: flex;
            align-items: center;
        }

        .task-bar {
            height: 28px;
            border-radius: 8px;
            position: absolute;
            transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
            display: flex;
            align-items: center;
            padding: 0 12px;
            font-size: 12px;
            font-weight: 600;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
            color: #ffffff;
            text-shadow: 0 1px 2px rgba(0, 0, 0, 0.4);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.2);
            cursor: text;
            user-select: none;
        }

        .task-bar:focus {
            outline: 2px solid white;
            outline-offset: 2px;
            z-index: 10;
            overflow: visible;
            min-width: fit-content;
            user-select: auto;
        }

        .month-labels {
            position: absolute;
            top: 0px;
            left: 0;
            width: 100%;
            height: 30px;
            color: var(--text-main);
            font-size: 14px;
            font-weight: 700;
        }
    </style>
</head>

<body>

    <div class="simulator-card">
        <div class="header">
            <div class="title-area" style="display: flex; flex-direction: column; gap: 10px; align-items: flex-start;">
                <h1 contenteditable="true" spellcheck="false" class="editable-box">Gantt Chart Maker</h1>
                <p contenteditable="true" spellcheck="false" class="editable-box"
                    style="margin: 0; color: var(--text-dim); font-size: 14px;">Create dynamic project timelines
                    instantly.</p>
            </div>
            <div class="stats-area">
                <div class="stat-box">
                    <div class="stat-label editable-box" contenteditable="true" spellcheck="false"
                        style="margin-bottom: 5px;">Total Tasks</div>
                    <div class="stat-value editable-box" id="total-tasks" contenteditable="true" spellcheck="false"
                        style="display: block;">0</div>
                </div>
                <div class="stat-box">
                    <div class="stat-label editable-box" contenteditable="true" spellcheck="false"
                        style="margin-bottom: 5px;">Total Weeks</div>
                    <div class="stat-value">
                        <input type="number" id="custom-total-weeks" value="26" min="1" class="editable-box"
                            title="Change total weeks visible" onchange="updateChart()"
                            style="width: 70px; text-align: center; font-size: 18px; font-weight: bold;">
                        <span class="editable-box" contenteditable="true" spellcheck="false"
                            style="font-size: 14px; color: var(--text-dim); font-weight: bold; margin-left: 5px;">Wks</span>
                    </div>
                </div>
            </div>
        </div>

        <div class="controls-panel">
            <div class="form-group">
                <label class="editable-box" contenteditable="true" spellcheck="false">Task Name</label>
                <input type="text" id="input-name" placeholder="E.g. Strategy Formulation">
            </div>
            <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 10px;">
                <div class="form-group">
                    <label class="editable-box" contenteditable="true" spellcheck="false">Start (Week)</label>
                    <input type="number" id="input-start" value="0" min="0">
                </div>
                <div class="form-group">
                    <label class="editable-box" contenteditable="true" spellcheck="false">Duration (Wks)</label>
                    <input type="number" id="input-dur" value="2" min="1">
                </div>
            </div>
            <div class="form-group">
                <label class="editable-box" contenteditable="true" spellcheck="false">Color</label>
                <select id="input-color">
                    <option value="var(--primary-blue)">Blue</option>
                    <option value="var(--primary-green)">Green</option>
                    <option value="var(--primary-orange)">Orange</option>
                    <option value="var(--primary-pink)">Pink</option>
                    <option value="var(--primary-purple)">Purple</option>
                </select>
            </div>
            <!-- Editable Add Button -->
            <button class="btn editable-box" id="add-task-btn" contenteditable="true" spellcheck="false">+ Add
                Task</button>

            <div class="editable-box" contenteditable="true" spellcheck="false"
                style="margin-top: 10px; font-size: 12px; color: var(--text-dim); font-weight: 600; text-transform: uppercase; letter-spacing: 1px; align-self: flex-start;">
                Task List
            </div>
            <div class="task-list" id="task-list-container">
                <!-- Tasks will appear here -->
            </div>
        </div>

        <div class="chart-panel">
            <div class="chart-container">
                <div class="chart-inner" id="chart">
                    <div class="month-labels" id="month-labels">
                        <!-- Dynamically generated month labels -->
                    </div>
                    <div id="grid-lines"></div>
                    <div id="tasks-container"></div>
                </div>
            </div>
        </div>

    </div>

    <script>
        const tasksContainer = document.getElementById('tasks-container');
        const taskListContainer = document.getElementById('task-list-container');
        const gridLines = document.getElementById('grid-lines');
        const monthLabels = document.getElementById('month-labels');
        const chartInner = document.getElementById('chart');
        const totalWeeksInput = document.getElementById('custom-total-weeks');

        let tasksData = [
            { id: 1, name: 'Project Kickoff', start: 0, dur: 2, color: 'var(--primary-blue)' },
            { id: 2, name: 'Design Phase', start: 2, dur: 4, color: 'var(--primary-purple)' },
            { id: 3, name: 'Development', start: 6, dur: 8, color: 'var(--primary-green)' }
        ];
        let nextId = 4;

        function updateTask(id, field, value) {
            const task = tasksData.find(t => t.id === id);
            if (task) {
                if (field === 'start' || field === 'dur') {
                    task[field] = parseInt(value) || (field === 'dur' ? 1 : 0);
                } else {
                    task[field] = value;
                }
                updateChart();
            }
        }

        // Prevents button default inner click causing issues while editing text
        document.getElementById('add-task-btn').addEventListener('click', function (e) {
            if (document.activeElement === this) return; // if currently focused text editing, don't execute
            addTask();
        });

        function renderTasksList() {
            taskListContainer.innerHTML = '';
            tasksData.forEach(task => {
                const el = document.createElement('div');
                el.className = 'task-item';
                el.innerHTML = `
                <div class="task-item-info" style="flex: 1; margin-right: 12px; min-width: 0;">
                    <input type="text" class="inline-input" value="${task.name.replace(/"/g, '&quot;')}" onchange="updateTask(${task.id}, 'name', this.value)" style="color: ${task.color}; font-size: 13px; font-weight: 700; width: 100%; margin-bottom: 4px; text-overflow: ellipsis;">
                    <div style="display: flex; gap: 8px; font-size: 11px; color: var(--text-dim); align-items: center; flex-wrap: wrap;">
                        <span style="white-space: nowrap; display: flex; align-items: center; gap: 4px;" class="editable-box" contenteditable="true" spellcheck="false">
                            Wk <input type="number" class="inline-input" value="${task.start}" min="0" onchange="updateTask(${task.id}, 'start', this.value)" style="width: 45px; color: white; display: inline-block;">
                        </span>
                        <span style="white-space: nowrap;" class="editable-box" contenteditable="true" spellcheck="false">
                            - Wk ${task.start + task.dur}
                        </span>
                        <span style="white-space: nowrap; display: flex; align-items: center; gap: 4px;" class="editable-box" contenteditable="true" spellcheck="false">
                            (<input type="number" class="inline-input" value="${task.dur}" min="1" onchange="updateTask(${task.id}, 'dur', this.value)" style="width: 45px; color: white; display: inline-block;"> wks)
                        </span>
                        <select class="inline-input" onchange="updateTask(${task.id}, 'color', this.value)" style="margin-left: auto; color: white; cursor: pointer;">
                            <option style="background: var(--bg-dark);" value="var(--primary-blue)" ${task.color === 'var(--primary-blue)' ? 'selected' : ''}>Blue</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-green)" ${task.color === 'var(--primary-green)' ? 'selected' : ''}>Green</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-orange)" ${task.color === 'var(--primary-orange)' ? 'selected' : ''}>Orange</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-pink)" ${task.color === 'var(--primary-pink)' ? 'selected' : ''}>Pink</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-purple)" ${task.color === 'var(--primary-purple)' ? 'selected' : ''}>Purple</option>
                        </select>
                    </div>
                </div>
                <button class="task-item-delete" title="Delete Task" onclick="deleteTask(${task.id})">&times;</button>
            `;
                taskListContainer.appendChild(el);
            });
        }

        function addTask() {
            const nameNode = document.getElementById('input-name');
            const startNode = document.getElementById('input-start');
            const durNode = document.getElementById('input-dur');
            const colorNode = document.getElementById('input-color');

            const name = nameNode.value.trim() || 'Untitled Task';
            const start = parseInt(startNode.value) || 0;
            const dur = parseInt(durNode.value) || 1;
            const color = colorNode.value;

            tasksData.push({ id: nextId++, name, start, dur, color });

            nameNode.value = '';
            startNode.value = (start + dur).toString();
            durNode.value = '2';

            updateChart();
        }

        function deleteTask(id) {
            tasksData = tasksData.filter(t => t.id !== id);
            updateChart();
        }

        function updateChart() {
            renderTasksList();

            let maxReach = 0;
            tasksData.forEach(t => {
                maxReach = Math.max(maxReach, t.start + t.dur);
            });

            // Use custom total weeks directly
            let customTotalWeeks = parseInt(totalWeeksInput.value) || 26;
            const chartWeeks = customTotalWeeks;

            const scale = 100 / chartWeeks; // Percent per week

            // Adjust chart width dynamically. 100% min means it doesn't shrink smaller than container
            chartInner.style.width = Math.max(100, (chartWeeks / 26) * 100) + '%';

            // Rebuild Grid
            gridLines.innerHTML = '';
            for (let i = 0; i <= chartWeeks; i += 2) {
                const line = document.createElement('div');
                line.className = 'grid-line';
                line.style.left = (i * scale) + '%';
                line.innerHTML = `<span class="editable-box" style="position: absolute; top: 35px; transform: translateX(-50%); font-size: 12px; font-weight: 600; color: var(--text-dim); text-align: center; background: var(--bg-dark);" contenteditable="true" spellcheck="false">Wk ${i}</span>`;
                gridLines.appendChild(line);
            }

            // Rebuild Month Labels
            monthLabels.innerHTML = '';
            for (let i = 0; i < chartWeeks; i += 4) {
                const monthNum = Math.floor(i / 4) + 1;
                const span = document.createElement('span');
                span.className = 'editable-box';
                span.contentEditable = "true";
                span.spellcheck = false;
                span.innerText = `Month ${monthNum}`;
                span.style.position = 'absolute';
                span.style.left = (i * scale) + '%';
                span.style.width = (Math.min(4, chartWeeks - i) * scale) + '%';
                span.style.textAlign = 'center';
                span.style.background = 'var(--bg-dark)';
                monthLabels.appendChild(span);
            }

            // Render Tasks
            tasksContainer.innerHTML = '';
            tasksData.forEach((task, index) => {
                const row = document.createElement('div');
                row.className = 'task-row';
                row.style.top = (index * 52 + 80) + 'px'; // Starts below axis labels

                const bar = document.createElement('div');
                bar.className = 'task-bar';
                bar.style.left = (task.start * scale) + '%';
                bar.style.width = (task.dur * scale) + '%';
                bar.style.backgroundColor = task.color;
                bar.innerText = task.name;
                bar.contentEditable = "true";
                bar.spellcheck = false;

                bar.onblur = function () {
                    updateTask(task.id, 'name', bar.innerText);
                };
                bar.onkeydown = function (e) {
                    if (e.key === 'Enter') {
                        e.preventDefault();
                        bar.blur();
                    }
                };

                row.appendChild(bar);
                tasksContainer.appendChild(row);
            });

            // Only update total tasks if the user hasn't typed their own manual override inside it
            const tt = document.getElementById('total-tasks');
            if (!tt.hasAttribute('data-customized')) {
                tt.innerText = tasksData.length;
            }

            const rw = document.getElementById('real-total-weeks');
            if (!rw.hasAttribute('data-customized')) {
                rw.innerText = maxReach + ' Wks';
            }
        }

        document.getElementById('total-tasks').addEventListener('input', function () {
            this.setAttribute('data-customized', 'true');
        });

        document.getElementById('real-total-weeks').addEventListener('input', function () {
            this.setAttribute('data-customized', 'true');
        });

        // Init
        updateChart();
    </script>
</body>

</html>