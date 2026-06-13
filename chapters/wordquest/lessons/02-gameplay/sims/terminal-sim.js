let initialized = false;

// Game State
const targetWord = "SOCKETS";
let guessed = new Set();
let message = "Welcome to WordQuest! Type a letter to guess.";
let score = 0;
let isFocused = document.hasFocus();
let blink = true;

setInterval(() => blink = !blink, 500);

window.bkSetup(1600, 900, (ctx, w, h) => {
	if (!initialized) {
		const canvas = ctx.canvas;
		canvas.style.touchAction = "none";
		
		window.addEventListener("focus", () => isFocused = true);
		window.addEventListener("blur", () => isFocused = false);
		
		window.addEventListener("keydown", (e) => {
			if (e.key.length === 1 && e.key.match(/[a-z]/i)) {
				const char = e.key.toUpperCase();
				if (!guessed.has(char)) {
					guessed.add(char);
					if (targetWord.includes(char)) {
						score++;
						message = `Good guess! '${char}' is in the word.`;
					} else {
						// Apply penalty if strict mode is on
						const isStrict = window.__simProps && window.__simProps.strictMode;
						if (isStrict) score--;
						message = `Oops! '${char}' is not in the word.`;
					}
				} else {
					message = `You already guessed '${char}'.`;
				}
			}
		});
		initialized = true;
	}
	
	// Dynamic Theming
	const isDark = (window.bkThemeMode && window.bkThemeMode() === "dark");
	let bg = window.bkColor ? window.bkColor("bg") : (isDark ? "#000" : "#fff");
	let paper = window.bkColor ? window.bkColor("paper") : (isDark ? "#111" : "#f5f5f5");
	let line = window.bkColor ? window.bkColor("line-strong") : (isDark ? "#333" : "#ccc");
	let text = window.bkColor ? window.bkColor("text") : (isDark ? "#fff" : "#000");
	let textLight = window.bkColor ? window.bkColor("text-light") : (isDark ? "#888" : "#666");
	let accent = window.bkColor ? window.bkColor("accent") : "#27c93f";
	const uiMode = window.bkUi ? window.bkUi() : "standard";
	
	// Contrast fallback
	if (isDark && text === "#000000") text = "#ffffff";
	if (!isDark && text === "#ffffff") text = "#000000";
	if (isDark && textLight === "#000000") textLight = "#888888";
	if (!isDark && textLight === "#ffffff") textLight = "#666666";

	ctx.fillStyle = bg;
	ctx.fillRect(0, 0, w, h);
	
	// Draw terminal window
	ctx.fillStyle = paper;
	ctx.strokeStyle = isFocused ? accent : line;
	ctx.lineWidth = uiMode === "neo" ? 10 : 4;
	
	ctx.beginPath();
	let radius = 10;
	if (uiMode === "playful") radius = 30;
	if (uiMode === "neo") radius = 0;
	
	ctx.roundRect(100, 100, 1400, 700, radius);
	ctx.fill();
	ctx.stroke();
	
	// Draw window buttons (macOS headlights)
	ctx.globalAlpha = isFocused ? 1.0 : 0.4;
	if (uiMode === "playful") {
		ctx.fillStyle = "#ff5f56"; ctx.beginPath(); ctx.arc(150, 150, 16, 0, Math.PI*2); ctx.fill();
		ctx.fillStyle = "#ffbd2e"; ctx.beginPath(); ctx.arc(190, 150, 16, 0, Math.PI*2); ctx.fill();
		ctx.fillStyle = "#27c93f"; ctx.beginPath(); ctx.arc(230, 150, 16, 0, Math.PI*2); ctx.fill();
	} else if (uiMode === "neo") {
		ctx.fillStyle = line; ctx.fillRect(130, 134, 24, 24);
		ctx.fillStyle = line; ctx.fillRect(170, 134, 24, 24);
		ctx.fillStyle = line; ctx.fillRect(210, 134, 24, 24);
	} else {
		ctx.fillStyle = "#ff5f56"; ctx.beginPath(); ctx.arc(140, 140, 10, 0, Math.PI*2); ctx.fill();
		ctx.fillStyle = "#ffbd2e"; ctx.beginPath(); ctx.arc(170, 140, 10, 0, Math.PI*2); ctx.fill();
		ctx.fillStyle = "#27c93f"; ctx.beginPath(); ctx.arc(200, 140, 10, 0, Math.PI*2); ctx.fill();
	}
	ctx.globalAlpha = 1.0;
	
	// Terminal Content
	ctx.fillStyle = text;
	ctx.font = "bold 32px 'JetBrains Mono', monospace, sans-serif";
	
	let y = 250;
	ctx.fillText("--- WORDQUEST TERMINAL ---", 140, y);
	y += 60;
	
	ctx.fillText(`Score: ${score}`, 140, y);
	y += 80;
	
	// Render Word
	let displayWord = "";
	let isWin = true;
	for (const char of targetWord) {
		if (guessed.has(char)) {
			displayWord += char + " ";
		} else {
			displayWord += "_ ";
			isWin = false;
		}
	}
	
	ctx.font = "bold 64px 'JetBrains Mono', monospace, sans-serif";
	ctx.fillStyle = isWin ? accent : text;
	ctx.fillText(displayWord, 140, y);
	y += 100;
	
	ctx.font = "bold 32px 'JetBrains Mono', monospace, sans-serif";
	if (isWin) {
		ctx.fillText("YOU WIN! Refresh the page to play again.", 140, y);
	} else {
		ctx.fillStyle = message.includes("Oops") ? "#ff5f56" : text;
		ctx.fillText(message, 140, y);
		y += 60;
		ctx.fillStyle = text;
		ctx.fillText("Guess a letter: ", 140, y);
		if (isFocused && blink) {
			const textWidth = ctx.measureText("Guess a letter: ").width;
			ctx.fillStyle = accent;
			ctx.fillText("█", 140 + textWidth, y);
		} else if (!isFocused) {
			const textWidth = ctx.measureText("Guess a letter: ").width;
			ctx.fillStyle = textLight;
			ctx.fillText("█", 140 + textWidth, y);
			ctx.fillStyle = textLight;
			ctx.font = "italic 24px 'JetBrains Mono', monospace, sans-serif";
			ctx.fillText("(Click terminal to focus)", 140 + textWidth + 40, y);
		}
	}
	
	// Guessed Letters
	y += 100;
	ctx.fillStyle = textLight;
	ctx.fillText(`Used letters: ${Array.from(guessed).join(", ")}`, 140, y);
});
