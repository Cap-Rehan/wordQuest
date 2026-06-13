import { lesson } from "mr-md";

export const introLesson = lesson("Introduction & Setup", { contentBase: import.meta.dir }, ctx => {
	ctx.description("A multiplayer terminal word-guessing game over TCP sockets, built in C.");
	ctx.add("content/intro.md");
	ctx.warning("content/warning.md");
});
