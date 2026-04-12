import type { ShikiTransformer } from 'shiki'
import { defineShikiSetup } from '@slidev/types'

/**
 * Custom Shiki transformer: word-level highlighting via inline comments.
 *
 * Add a comment to any code block:
 *   // highlight-word:[=]
 *
 * The transformer removes the comment line from the output and wraps
 * every literal occurrence of the word in <span class="highlighted-word">.
 * Works even when the word spans multiple Shiki tokens.
 */
function transformerHighlightWord(): ShikiTransformer {
    return {
        name: 'highlight-word',
        code(code) {
            const marker = /\/\/\s*highlight-word:(.+)/

            let word: string | undefined
            const lineToRemove: any[] = []

            for (const line of code.children) {
                if (line.type !== 'element') continue
                const text = flatText(line)
                const m = text.match(marker)
                if (m) {
                    word = m[1].trim()
                    lineToRemove.push(line)
                }
            }

            if (!word) return

            // Remove marker comment lines and trailing newlines
            for (const line of lineToRemove) {
                const idx = code.children.indexOf(line)
                if (idx !== -1) {
                    if (code.children[idx + 1]?.type === 'text' && code.children[idx + 1].value === '\n') {
                        code.children.splice(idx, 2)
                    } else {
                        code.children.splice(idx, 1)
                    }
                }
            }

            // Process each line element
            for (const line of code.children) {
                if (line.type !== 'element') continue
                const lineText = flatText(line)
                if (!lineText.includes(word)) continue
                line.children = highlightInLine(line.children, word)
            }
        },
    }
}

/** Extract plain text from a HAST node tree */
function flatText(node: any): string {
    if (node.type === 'text') return node.value ?? ''
    return (node.children ?? []).map((c: any) => flatText(c)).join('')
}

/**
 * Rebuild a line's children array, wrapping occurrences of `word`
 * in <span class="highlighted-word"> even when `word` spans multiple tokens.
 */
function highlightInLine(children: any[], word: string): any[] {
    // Flatten to a list of { text, node } segments (leaf text nodes with their parent style)
    const segments: { text: string; props: any }[] = []
    function collect(node: any, inheritedProps: any) {
        if (node.type === 'text') {
            segments.push({ text: node.value, props: inheritedProps })
        } else if (node.children) {
            const p = node.properties ?? inheritedProps
            for (const child of node.children) collect(child, p)
        }
    }
    for (const child of children) collect(child, {})

    // Build a flat string and map each char to its segment index and offset
    const fullText = segments.map(s => s.text).join('')
    const charToSeg: number[] = []
    const charToOffset: number[] = []
    segments.forEach((s, i) => {
        for (let j = 0; j < s.text.length; j++) {
            charToSeg.push(i)
            charToOffset.push(j)
        }
    })

    // Find all occurrences of word
    const matches: { start: number; end: number }[] = []
    let pos = 0
    while (true) {
        const idx = fullText.indexOf(word, pos)
        if (idx === -1) break
        matches.push({ start: idx, end: idx + word.length })
        pos = idx + word.length
    }

    if (matches.length === 0) return children

    // Rebuild: walk through chars, emitting styled spans and highlight wrappers
    const result: any[] = []
    let cur = 0

    function emitRange(from: number, to: number, highlight: boolean) {
        if (from >= to) return
        // Group consecutive chars by their segment (preserving syntax coloring)
        const spans: { props: any; text: string }[] = []
        for (let i = from; i < to; i++) {
            const seg = segments[charToSeg[i]]
            const ch = seg.text[charToOffset[i]]
            const last = spans.length > 0 ? spans[spans.length - 1] : null
            if (last && last.props === seg.props) {
                last.text += ch
            } else {
                spans.push({ props: seg.props, text: ch })
            }
        }

        const spanNodes = spans.map(s => ({
            type: 'element',
            tagName: 'span',
            properties: { ...s.props },
            children: [{ type: 'text', value: s.text }],
        }))

        if (highlight) {
            result.push({
                type: 'element',
                tagName: 'span',
                properties: { class: 'highlighted-word' },
                children: spanNodes,
            })
        } else {
            result.push(...spanNodes)
        }
    }

    for (const match of matches) {
        emitRange(cur, match.start, false)
        emitRange(match.start, match.end, true)
        cur = match.end
    }
    emitRange(cur, fullText.length, false)

    return result
}

export default defineShikiSetup(() => {
    return {
        transformers: [
            transformerHighlightWord(),
        ],
    }
})
