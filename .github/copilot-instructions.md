# Capture This - a Talk aboout a C++ feature

## Summary

This is a talk and example code about capturing this in C++ lambdas

## Persisting Discussions to Log Files

When the user asks to persist a discussion, decision, or analysis to a log file (typically under `logs/`):

1. **Append verbatim.** Write the full, long-form version of the conversation exactly as it appeared in chat. Do NOT rephrase, shorten, summarize, or paraphrase answers. The user expects to find the complete reasoning and analysis in the file.
2. **Include both sides.** Reproduce the user's messages (as blockquotes `>`) and the assistant's full responses, including all tables, code blocks, pro/con lists, and recommendations.
3. **Append at the end.** New content is always appended to the end of the existing log file, separated by a horizontal rule (`---`). Never overwrite or reorganize earlier content.
4. **Maintain markdown structure.** Use proper headings, tables, and code fences consistent with the rest of the file.
5. **Persist decisions immediately.** When the user makes a design decision during a conversation, append it to the active log file right away — don't wait to be asked.

### Automatic Persistence Rule

**Every substantive answer** (detailed explanations, architecture discussions, design decisions, troubleshooting analyses, Q&A exchanges) **MUST be persisted to the active session log file.** The active log file is the most recent file under `logs/` that matches the current session topic. If unsure which file, ask.

This applies to ALL multi-paragraph technical responses — not just when the user explicitly says "persist". The log is the project's institutional memory; if an answer isn't persisted, it's lost.

### Persistence Workflow (mandatory)

To prevent persistence from being forgotten under cognitive load, follow this strict order:

1. **Before answering**, add a todo item: "Persist to log" (use the todo list tool).
2. **Write the substantive content directly into the log file first** — compose the full answer (with heading, blockquoted user message, and response body) as an append to the log file. This is the _primary_ artifact.
3. **Then respond to the user** with the same content or a concise summary referencing what was persisted.
4. **Mark the "Persist to log" todo as completed.**

The inversion matters: writing to the log is step 2, _before_ the chat reply. If the turn is interrupted or cut short, the log already has the content.

### What Counts as Substantive

Persist if the response contains **any** of:
- A code block or compiler output
- A table
- More than two paragraphs of technical explanation
- A design decision or recommendation
- A new fact about C++ standards, Slidev, or the toolchain

Do **not** persist: one-line confirmations ("Done."), file-edit acknowledgements, or purely mechanical tool output.
