import { lesson } from "mr-md";

export const gameplayLesson = lesson("How to Play", { contentBase: import.meta.dir }, ctx => {
	ctx.description("Learn how to connect clients and the rules of WordQuest.");
	ctx.lab("sims/terminal-sim.js", { label: "WordQuest Demo" });
	ctx.add("content/gameplay.md");
	ctx.quiz("quizzes/rules.json", { label: "Rules Check", caption: "Test your knowledge of the game rules!" });
});
