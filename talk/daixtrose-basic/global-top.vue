<script setup lang="ts">
import { onMounted, onUnmounted } from 'vue'
import { useNav } from '@slidev/client'

const { next, prev } = useNav()

let lastWheelTime = 0
const DEBOUNCE_MS = 300

function onWheel(e: WheelEvent) {
  const now = Date.now()
  if (now - lastWheelTime < DEBOUNCE_MS) return
  lastWheelTime = now

  if (e.deltaY > 0) {
    next()
  } else if (e.deltaY < 0) {
    prev()
  }
}

onMounted(() => {
  document.addEventListener('wheel', onWheel, { passive: true })
})

onUnmounted(() => {
  document.removeEventListener('wheel', onWheel)
})
</script>

<template>
  <div />
</template>
