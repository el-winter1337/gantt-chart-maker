
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
        let currentNoteTaskId = null;

        function openNotesModal(taskId, event) {
            const task = tasksData.find(t => t.id === taskId);
            if (!task) return;
            currentNoteTaskId = taskId;
            document.getElementById('notes-modal-title').value = task.name;
            document.getElementById('notes-modal-textarea').value = task.notes || '';
            
            const modal = document.getElementById('notes-modal');
            modal.style.display = 'block';
            
            let left = event.clientX + 15;
            let top = event.clientY + 15;
            
            if (left + 320 > window.innerWidth) left = window.innerWidth - 340;
            if (top + 220 > window.innerHeight) top = event.clientY - 230;
            
            modal.style.left = left + 'px';
            modal.style.top = top + 'px';
            
            setTimeout(() => document.getElementById('notes-modal-textarea').focus(), 50);
        }

        function closeNotesModal() {
            document.getElementById('notes-modal').style.display = 'none';
            currentNoteTaskId = null;
        }

        function saveNotes() {
            if (currentNoteTaskId !== null) {
                const task = tasksData.find(t => t.id === currentNoteTaskId);
                if (task) {
                    task.name = document.getElementById('notes-modal-title').value.trim() || 'Untitled Task';
                    task.notes = document.getElementById('notes-modal-textarea').value;
                }
            }
            closeNotesModal();
            updateChart();
        }

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

        document.getElementById('add-task-btn').addEventListener('click', function (e) {
            addTask();
        });

        function renderTasksList() {
            taskListContainer.innerHTML = '';
            tasksData.forEach(task => {
                const el = document.createElement('div');
                el.className = 'task-item';
                el.draggable = true;
                
                el.ondragstart = (e) => {
                    e.dataTransfer.setData('text/plain', task.id);
                    setTimeout(() => el.style.opacity = '0.4', 0);
                };

                el.ondragend = () => {
                    el.style.opacity = '1';
                    document.querySelectorAll('.task-item').forEach(item => item.style.boxShadow = '');
                };

                el.ondragover = (e) => {
                    e.preventDefault();
                    if (e.dataTransfer.types.includes('text/plain')) {
                        const rect = el.getBoundingClientRect();
                        const midY = rect.top + rect.height / 2;
                        el.style.boxShadow = e.clientY < midY ? '0 -2px 0 var(--primary-blue)' : '0 2px 0 var(--primary-blue)';
                    }
                };

                el.ondragleave = () => {
                    el.style.boxShadow = '';
                };

                el.ondrop = (e) => {
                    e.preventDefault();
                    el.style.boxShadow = '';
                    const draggedId = parseInt(e.dataTransfer.getData('text/plain'));
                    if (draggedId && draggedId !== task.id) {
                        const draggedIndex = tasksData.findIndex(t => t.id === draggedId);
                        let targetIndex = tasksData.findIndex(t => t.id === task.id);
                        if (draggedIndex !== -1 && targetIndex !== -1) {
                            const rect = el.getBoundingClientRect();
                            if (e.clientY > rect.top + rect.height / 2) targetIndex++;
                            if (draggedIndex < targetIndex) targetIndex--;
                            const [movedTask] = tasksData.splice(draggedIndex, 1);
                            tasksData.splice(targetIndex, 0, movedTask);
                            updateChart();
                        }
                    }
                };

                el.innerHTML = `
                <div style="cursor: grab; color: var(--text-dim); padding-right: 10px; font-weight: bold; font-size: 16px; user-select: none; display: flex; align-items: center;" title="Drag to reorder">&#x28FF;</div>
                <div class="task-item-info" style="flex: 1; margin-right: 12px; min-width: 0;">
                    <input type="text" class="inline-input" value="${task.name.replace(/"/g, '&quot;')}" onchange="updateTask(${task.id}, 'name', this.value)" style="color: ${task.color}; font-size: 13px; font-weight: 700; width: 100%; margin-bottom: 4px; text-overflow: ellipsis;">
                    <div style="display: flex; gap: 8px; font-size: 11px; color: var(--text-dim); align-items: center; flex-wrap: wrap;">
                        <span style="white-space: nowrap; display: flex; align-items: center; gap: 4px;" class="editable-box" contenteditable="true" spellcheck="false">
                            Wk <input type="number" class="inline-input" value="${task.start}" min="0" onchange="updateTask(${task.id}, 'start', this.value)" style="width: 45px; color: var(--text-main); display: inline-block;">
                        </span>
                        <span style="white-space: nowrap;" class="editable-box" contenteditable="true" spellcheck="false">
                            - Wk ${task.start + task.dur}
                        </span>
                        <span style="white-space: nowrap; display: flex; align-items: center; gap: 4px;" class="editable-box" contenteditable="true" spellcheck="false">
                            (<input type="number" class="inline-input" value="${task.dur}" min="1" onchange="updateTask(${task.id}, 'dur', this.value)" style="width: 45px; color: var(--text-main); display: inline-block;"> wks)
                        </span>
                        <select class="inline-input" onchange="updateTask(${task.id}, 'color', this.value)" style="margin-left: auto; color: var(--text-main); cursor: pointer;">
                            <option style="background: var(--bg-dark);" value="var(--primary-blue)" ${task.color === 'var(--primary-blue)' ? 'selected' : ''}>🔵 Blue</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-green)" ${task.color === 'var(--primary-green)' ? 'selected' : ''}>🟢 Green</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-orange)" ${task.color === 'var(--primary-orange)' ? 'selected' : ''}>🟠 Orange</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-pink)" ${task.color === 'var(--primary-pink)' ? 'selected' : ''}>🩷 Pink</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-purple)" ${task.color === 'var(--primary-purple)' ? 'selected' : ''}>🟣 Purple</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-red)" ${task.color === 'var(--primary-red)' ? 'selected' : ''}>🔴 Red</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-teal)" ${task.color === 'var(--primary-teal)' ? 'selected' : ''}>🌱 Teal</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-cyan)" ${task.color === 'var(--primary-cyan)' ? 'selected' : ''}>🩵 Cyan</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-yellow)" ${task.color === 'var(--primary-yellow)' ? 'selected' : ''}>🟡 Yellow</option>
                            <option style="background: var(--bg-dark);" value="var(--primary-indigo)" ${task.color === 'var(--primary-indigo)' ? 'selected' : ''}>🌀 Indigo</option>
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
                line.innerHTML = `<span class="editable-box" style="position: absolute; top: 35px; transform: translateX(-50%); font-size: 12px; font-weight: 600; color: var(--text-dim); text-align: center; background: var(--bg-dark); white-space: nowrap; z-index: 2;" contenteditable="true" spellcheck="false">Wk ${i}</span>`;
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
                span.style.minWidth = (Math.min(4, chartWeeks - i) * scale) + '%';
                span.style.textAlign = 'center';
                span.style.background = 'var(--bg-dark)';
                span.style.whiteSpace = 'nowrap';
                span.style.zIndex = '2';
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
                bar.title = "Click to add or view notes";
                
                if (task.notes && task.notes.trim() !== '') {
                    bar.innerHTML = task.name.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;') + ' <span style="margin-left: 6px; font-size: 11px;">📝</span>';
                } else {
                    bar.innerText = task.name;
                }

                bar.onclick = function (e) {
                    openNotesModal(task.id, e);
                    e.stopPropagation();
                };

                row.appendChild(bar);
                tasksContainer.appendChild(row);
            });

            // Only update total tasks if the user hasn't typed their own manual override inside it
            const tt = document.getElementById('total-tasks');
            if (tt && !tt.hasAttribute('data-customized')) {
                tt.innerText = tasksData.length;
            }

            const rw = document.getElementById('real-total-weeks');
            if (rw && !rw.hasAttribute('data-customized')) {
                rw.innerText = maxReach + ' Wks';
            }
        }

        const totalTasksEl = document.getElementById('total-tasks');
        if (totalTasksEl) {
            totalTasksEl.addEventListener('input', function () {
                this.setAttribute('data-customized', 'true');
            });
        }

        const realTotalWeeksEl = document.getElementById('real-total-weeks');
        if (realTotalWeeksEl) {
            realTotalWeeksEl.addEventListener('input', function () {
                this.setAttribute('data-customized', 'true');
            });
        }

        function exportChartAsPNG() {
            const chartNode = document.getElementById('chart');
            const originalBg = chartNode.style.background;
            
            // To ensure the PNG has a solid dark background instead of transparent
            const themeBg = document.body.getAttribute('data-theme') === 'light' ? '#f8fafc' : '#0f172a'; chartNode.style.background = themeBg;
            
            const btn = document.querySelector('button[onclick="exportChartAsPNG()"]');
            const originalText = btn.innerHTML;
            btn.innerHTML = '⏳ Exporting...';
            btn.style.pointerEvents = 'none';

            html2canvas(chartNode, {
                backgroundColor: themeBg,
                scale: window.devicePixelRatio > 1 ? window.devicePixelRatio : 2, // High resolution
                logging: false,
                useCORS: true
            }).then(canvas => {
                chartNode.style.background = originalBg;
                
                const link = document.createElement('a');
                link.download = 'Gantt_Timeline.png';
                link.href = canvas.toDataURL('image/png');
                link.click();
                
                btn.innerHTML = '✅ Exported!';
                setTimeout(() => {
                    btn.innerHTML = originalText;
                    btn.style.pointerEvents = 'auto';
                }, 2000);
            }).catch(err => {
                console.error('Error taking screenshot:', err);
                chartNode.style.background = originalBg;
                btn.innerHTML = '❌ Error';
                setTimeout(() => {
                    btn.innerHTML = originalText;
                    btn.style.pointerEvents = 'auto';
                }, 2000);
            });
        }

        function openAboutModal() {
            if (document.getElementById('about-modal-layer')) return;

            const overlay = document.createElement('div');
            overlay.id = 'about-modal-layer';
            overlay.style.cssText = `
                position: fixed; top: 0; left: 0; width: 100vw; height: 100vh;
                background: var(--modal-overlay); backdrop-filter: blur(5px);
                -webkit-backdrop-filter: blur(5px);
                z-index: 2000; display: flex; align-items: center; justify-content: center;
                opacity: 0; transition: opacity 0.2s ease;
            `;
            
            overlay.onclick = function(e) {
                if (e.target === overlay) {
                    overlay.style.opacity = '0';
                    modal.style.transform = 'scale(0.95)';
                    setTimeout(() => overlay.remove(), 200);
                }
            };

            const modal = document.createElement('div');
            modal.style.cssText = `
                background: var(--modal-bg-grad);
                border: 1px solid var(--border-color);
                padding: 40px 60px; border-radius: 16px;
                box-shadow: 0 25px 50px -12px var(--shadow-color);
                text-align: center;
                transform: scale(0.95); transition: transform 0.2s ease;
            `;
            
            modal.innerHTML = `
                <div style="font-size: 56px; margin-bottom: 20px; text-shadow: 0 0 20px var(--shadow-color);">❄️</div>
                <h3 style="margin: 0 0 10px 0; font-size: 26px; color: var(--text-main); font-weight: 600;">About</h3>
                <p style="margin: 0; font-size: 16px; color: var(--text-dim); letter-spacing: 0.5px;">created by winter❄️ / 2026.4.19</p>
                <div style="margin-top: 35px;">
                    <button class="nav-btn" style="margin: 0 auto; background: var(--btn-ghost-hover); padding: 10px 30px;" onclick="document.getElementById('about-modal-layer').click()">Close</button>
                </div>
            `;
            
            overlay.appendChild(modal);
            document.body.appendChild(overlay);
            
            setTimeout(() => {
                overlay.style.opacity = '1';
                modal.style.transform = 'scale(1)';
            }, 10);
        }

        function exportDBFile() {
            const staticEditables = [
                'project-title', 'project-subtitle',
                'label-total-tasks', 'total-tasks',
                'label-total-weeks', 'label-wks',
                'label-task-name', 'label-start-week',
                'label-duration', 'label-color', 'label-task-list'
            ];
            const editablesData = {};
            staticEditables.forEach(id => {
                const el = document.getElementById(id);
                if (el) editablesData[id] = el.innerHTML;
            });

            const data = {
                totalWeeks: document.getElementById('custom-total-weeks').value,
                tasksData: tasksData,
                editablesData: editablesData
            };
            const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' });
            const url = URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = 'gantt_data.db';
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            URL.revokeObjectURL(url);
        }

        function triggerImport() {
            document.getElementById('import-file-input').click();
        }

        function importDBFile(event) {
            const file = event.target.files[0];
            if (!file) return;

            const reader = new FileReader();
            reader.onload = function(e) {
                try {
                    const data = JSON.parse(e.target.result);
                    
                    if (data.totalWeeks !== undefined) {
                        document.getElementById('custom-total-weeks').value = data.totalWeeks;
                    }
                    if (data.tasksData && Array.isArray(data.tasksData)) {
                        tasksData = data.tasksData;
                        let maxId = 0;
                        tasksData.forEach(task => { if (task.id > maxId) maxId = task.id; });
                        nextId = maxId + 1;
                    }

                    // Must update chart to redraw UI based on changed state
                    updateChart();

                    if (data.editablesData) {
                        Object.keys(data.editablesData).forEach(id => {
                            const el = document.getElementById(id);
                            if (el) {
                                el.innerHTML = data.editablesData[id];
                                if (id === 'total-tasks') {
                                    el.setAttribute('data-customized', 'true');
                                }
                            }
                        });
                    }
                } catch (err) {
                    console.error('Error importing file:', err);
                    alert('Invalid database file.');
                }
                event.target.value = '';
            };
            reader.readAsText(file);
        }

        function toggleTheme() {
            const body = document.body;
            const btn = document.getElementById('theme-toggle');
            if (body.getAttribute('data-theme') === 'light') {
                body.removeAttribute('data-theme');
                btn.innerHTML = '☀️ White Theme';
            } else {
                body.setAttribute('data-theme', 'light');
                btn.innerHTML = '🌙 Dark Theme';
            }
            updateChart(); // redraw inline colors relying on theme variables if any
        }
        
        // Init
        updateChart();
    