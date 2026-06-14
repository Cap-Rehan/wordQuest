import { chapter } from "mr-md";
import { introLesson } from "./lessons/01-intro/lesson.js";
import { gameplayLesson } from "./lessons/02-gameplay/lesson.js";
import { architectureLesson } from "./lessons/03-architecture/lesson.js";

export const wordquestChapter = chapter("WordQuest Documentation", ctx => {
	ctx.lesson(introLesson);
	ctx.lesson(gameplayLesson);
	ctx.lesson(architectureLesson);
});

if (import.meta.main) {
	wordquestChapter.build();
}
