import { lesson } from "mr-md";

export const architectureLesson = lesson("Architecture & Files", { contentBase: import.meta.dir }, ctx => {
	ctx.description("A high-level overview of the WordQuest C source code.");
	ctx.add("content/arch.md");
	ctx.columns([ { src: "content/arch-server.md" }, { src: "content/arch-client.md" } ]);
	ctx.tip("content/tip.md");
});
