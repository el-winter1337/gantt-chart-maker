import re
import sys

try:
    with open(r"c:\Users\shres\Downloads\gannt chart\core chart copy.html", "r", encoding="utf-8") as f:
        content = f.read()

    # 1. Update CSS variables and [data-theme="light"]
    new_root = """:root {
            /* Base Colors */
            --bg-dark: #0f172a;
            --card-bg: rgba(30, 41, 59, 0.75);
            --text-main: #f8fafc;
            --text-dim: #94a3b8;
            --page-bg: linear-gradient(135deg, #020617 0%, #172554 100%);
            --nav-bg: rgba(15, 23, 42, 0.4);
            
            /* Overlays (mostly semantic for dark/light) */
            --glass-bg: rgba(255, 255, 255, 0.03);
            --glass-border: rgba(255, 255, 255, 0.05);
            --glass-bg-hover: rgba(255, 255, 255, 0.06);
            --glass-border-hover: rgba(255, 255, 255, 0.1);
            
            --input-bg: rgba(0, 0, 0, 0.2);
            --input-border: rgba(255, 255, 255, 0.08);
            --input-bg-hover: rgba(0, 0, 0, 0.4);
            --input-border-hover: rgba(255, 255, 255, 0.3);
            --input-bg-focus: rgba(0, 0, 0, 0.5);
            
            --btn-ghost-bg: rgba(255, 255, 255, 0.05);
            --btn-ghost-border: rgba(255, 255, 255, 0.1);
            --btn-ghost-hover: rgba(255, 255, 255, 0.1);
            --btn-ghost-hover-border: rgba(255, 255, 255, 0.2);
            
            --title-grad-start: #fff;
            --title-grad-end: #93c5fd;
            
            --modal-bg-grad: linear-gradient(135deg, #1e293b, #0f172a);
            --modal-overlay: rgba(0, 0, 0, 0.6);
            --shadow-color: rgba(0, 0, 0, 0.2);
            --grid-line-color: rgba(255, 255, 255, 0.1);
            --border-color: rgba(255, 255, 255, 0.1);
            --scrollbar-thumb: rgba(255, 255, 255, 0.2);
            --scrollbar-thumb-hover: rgba(255, 255, 255, 0.3);

            /* Primary Colors */
            --primary-blue: #3b82f6;
            --primary-green: #10b981;
            --primary-orange: #f59e0b;
            --primary-pink: #ec4899;
            --primary-purple: #8b5cf6;
            --primary-red: #ef4444;
            --primary-teal: #14b8a6;
            --primary-cyan: #06b6d4;
            --primary-yellow: #eab308;
            --primary-indigo: #6366f1;
        }

        [data-theme="light"] {
            --bg-dark: #f8fafc;
            --card-bg: rgba(255, 255, 255, 0.85);
            --text-main: #0f172a;
            --text-dim: #64748b;
            --page-bg: linear-gradient(135deg, #f1f5f9 0%, #e2e8f0 100%);
            --nav-bg: rgba(255, 255, 255, 0.6);

            --glass-bg: rgba(0, 0, 0, 0.02);
            --glass-border: rgba(0, 0, 0, 0.1);
            --glass-bg-hover: rgba(0, 0, 0, 0.05);
            --glass-border-hover: rgba(0, 0, 0, 0.15);
            
            --input-bg: rgba(0, 0, 0, 0.03);
            --input-border: rgba(0, 0, 0, 0.1);
            --input-bg-hover: rgba(0, 0, 0, 0.06);
            --input-border-hover: rgba(0, 0, 0, 0.2);
            --input-bg-focus: #fff;
            
            --btn-ghost-bg: rgba(0, 0, 0, 0.05);
            --btn-ghost-border: rgba(0, 0, 0, 0.1);
            --btn-ghost-hover: rgba(0, 0, 0, 0.08);
            --btn-ghost-hover-border: rgba(0, 0, 0, 0.15);
            
            --title-grad-start: #1e3a8a;
            --title-grad-end: #2563eb;
            
            --modal-bg-grad: linear-gradient(135deg, #ffffff, #f8fafc);
            --modal-overlay: rgba(255, 255, 255, 0.4);
            --shadow-color: rgba(0, 0, 0, 0.08);
            --grid-line-color: rgba(0, 0, 0, 0.1);
            --border-color: rgba(0, 0, 0, 0.1);
            --scrollbar-thumb: rgba(0, 0, 0, 0.15);
            --scrollbar-thumb-hover: rgba(0, 0, 0, 0.25);
        }"""
    
    # replace root block
    root_pattern = r':root\s*\{.*?\border-color:\s*rgba\(255,\s*255,\s*255,\s*0\.1\);\s*\}'
    content = re.sub(root_pattern, new_root, content, flags=re.DOTALL)
    
    # 2. Swap CSS hardcoded values with variables
    replacements = {
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.2\)': r'background: var(--scrollbar-thumb)',
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.3\)': r'background: var(--scrollbar-thumb-hover)',
        r'background:\s*linear-gradient\(135deg,\s*#020617\s*0%,\s*#172554\s*100%\)': r'background: var(--page-bg)',
        r'background:\s*rgba\(15,\s*23,\s*42,\s*0\.4\)': r'background: var(--nav-bg)',
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.05\)': r'background: var(--btn-ghost-bg)',
        r'border:\s*1px\s*solid\s*rgba\(255,\s*255,\s*255,\s*0\.1\)': r'border: 1px solid var(--btn-ghost-border)',
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.1\)': r'background: var(--btn-ghost-hover)',
        r'border-color:\s*rgba\(255,\s*255,\s*255,\s*0\.2\)': r'border-color: var(--btn-ghost-hover-border)',
        r'background:\s*rgba\(0,\s*0,\s*0,\s*0\.2\)': r'background: var(--input-bg)',
        r'border:\s*1px\s*solid\s*rgba\(255,\s*255,\s*255,\s*0\.08\)': r'border: 1px solid var(--input-border)',
        r'background:\s*rgba\(0,\s*0,\s*0,\s*0\.4\)': r'background: var(--input-bg-hover)',
        r'border-color:\s*rgba\(255,\s*255,\s*255,\s*0\.3\)': r'border-color: var(--input-border-hover)',
        r'background:\s*rgba\(0,\s*0,\s*0,\s*0\.5\)': r'background: var(--input-bg-focus)',
        r'background:\s*linear-gradient\(90deg,\s*#fff,\s*#93c5fd\)': r'background: linear-gradient(90deg, var(--title-grad-start), var(--title-grad-end))',
        r'border-bottom:\s*1px\s*solid\s*rgba\(255,\s*255,\s*255,\s*0\.08\)': r'border-bottom: 1px solid var(--border-color)',
        r'border:\s*1px\s*solid\s*rgba\(255,\s*255,\s*255,\s*0\.05\)': r'border: 1px solid var(--glass-border)',
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.03\)': r'background: var(--glass-bg)',
        r'background:\s*rgba\(255,\s*255,\s*255,\s*0\.06\)': r'background: var(--glass-bg-hover)',
        r'border-color:\s*rgba\(255,\s*255,\s*255,\s*0\.1\)': r'border-color: var(--glass-border-hover)',
        r'border-left:\s*1px\s*dashed\s*rgba\(255,\s*255,\s*255,\s*0\.1\)': r'border-left: 1px dashed var(--grid-line-color)',
        r'box-shadow:\s*0\s*4px\s*12px\s*rgba\(0,\s*0,\s*0,\s*0\.2\)': r'box-shadow: 0 4px 12px var(--shadow-color)',
        r'box-shadow:\s*0\s*6px\s*16px\s*rgba\(0,\s*0,\s*0,\s*0\.3\)': r'box-shadow: 0 6px 16px var(--shadow-color)',
        r'text-shadow:\s*0\s*1px\s*2px\s*rgba\(0,\s*0,\s*0,\s*0\.4\)': r'text-shadow: 0 1px 2px var(--shadow-color)',
        r'background:\s*rgba\(239,\s*68,\s*68,\s*0\.1\)': r'background: rgba(239, 68, 68, 0.15)',
        r'border-bottom:\s*1px\s*solid\s*var\(--border-color\)': r'border-bottom: 1px solid var(--border-color)',
    }
    for old, new_ in replacements.items():
        content = re.sub(old, new_, content)

    # Convert static string colors inside inline styles explicitly to use variables
    # e.g background: rgba(0, 0, 0, 0.3) manually added in HTML 
    content = re.sub(r'style="([^"]*)background:\s*rgba\(0,\s*0,\s*0,\s*0\.3\)', r'style="\1background: var(--input-bg-hover)', content)
    content = re.sub(r'style="([^"]*)border:\s*1px\s*solid\s*rgba\(255,\s*255,\s*255,\s*0\.2\)', r'style="\1border: 1px solid var(--glass-border)', content)

    # 3. Add Theme Toggle button
    navbar_target = '<div class="top-navbar">'
    navbar_replacement = '<div class="top-navbar">\n        <button class="nav-btn" onclick="toggleTheme()" id="theme-toggle">☀️ White Theme</button>'
    content = content.replace(navbar_target, navbar_replacement)

    # 4. Inject toggleTheme JS function + inject dynamic background for html2canvas
    js_target = '// Init'
    js_replacement = """function toggleTheme() {
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
        
        // Init"""
    if 'function toggleTheme()' not in content:
        content = content.replace(js_target, js_replacement)

    # 5. Fix exportChartAsPNG solid background mapping
    export_target = "chartNode.style.background = '#0f172a';"
    export_replacement = "const themeBg = document.body.getAttribute('data-theme') === 'light' ? '#f8fafc' : '#0f172a'; chartNode.style.background = themeBg;"
    content = content.replace(export_target, export_replacement)
    
    export_canvas_target = "backgroundColor: '#0f172a',"
    export_canvas_replacement = "backgroundColor: themeBg,"
    content = content.replace(export_canvas_target, export_canvas_replacement)
    
    # 6. Some inline dark styles inside JS
    # In JS: 'color: white;' might be hardcoded, replace it with nothing or var(--text-main)
    # Be careful for task.color === 'white' cases. I will replace specific instances.
    content = content.replace("color: white; display: inline-block;", "color: var(--text-main); display: inline-block;") 
    content = content.replace("color: white; background: rgba(0, 0, 0, 0.2);", "color: var(--text-main); background: var(--input-bg);")
    
    # 7. Write back
    with open(r"c:\Users\shres\Downloads\gannt chart\core chart copy.html", "w", encoding="utf-8") as f:
        f.write(content)
        
    print("DONE")

except Exception as e:
    print("ERROR:", e)
