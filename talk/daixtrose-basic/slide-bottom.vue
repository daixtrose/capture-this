<script setup lang="ts">
import { computed, inject, onMounted, ref, watch } from "vue";
import { resolveAssetUrl } from "@slidev/theme-default/layoutHelper";

const frontmatter = inject<Record<string, any>>("$$slidev-fontmatter", {});

const background = computed(() => frontmatter.background || "");
const dim = computed(() => frontmatter.dim === true);

const isBright = ref(false);
const measured = ref(false);
const el = ref<HTMLElement>();

const isImage = computed(() => {
  return (
    background.value &&
    !["#", "rgb", "hsl"].some((v) => background.value.indexOf(v) === 0)
  );
});

function measureBrightness(url: string) {
  const img = new Image();
  img.crossOrigin = "anonymous";
  img.onload = () => {
    const canvas = document.createElement("canvas");
    const size = 64;
    canvas.width = size;
    canvas.height = size;
    const ctx = canvas.getContext("2d")!;
    ctx.drawImage(img, 0, 0, size, size);
    const data = ctx.getImageData(0, 0, size, size).data;
    let totalLuminance = 0;
    const pixelCount = data.length / 4;
    for (let i = 0; i < data.length; i += 4) {
      totalLuminance +=
        0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2];
    }
    isBright.value = totalLuminance / pixelCount > 110;
    measured.value = true;
  };
  img.src = url;
}

watch(
  background,
  (bg) => {
    measured.value = false;
    if (bg && isImage.value) {
      measureBrightness(resolveAssetUrl(bg));
    }
  },
  { immediate: true },
);

function applyClasses() {
  const parent = el.value?.parentElement;
  if (!parent) return;
  parent.classList.remove("bg-bright", "bg-dark", "has-background");
  if (background.value) {
    parent.classList.add("has-background");
    if (measured.value) {
      parent.classList.add(isBright.value ? "bg-bright" : "bg-dark");
    }
  }
}

watch([measured, isBright, background], applyClasses);
onMounted(applyClasses);

const bgStyle = computed(() => {
  if (!background.value) return { display: "none" };
  const isColor = ["#", "rgb", "hsl"].some(
    (v) => background.value.indexOf(v) === 0,
  );
  return {
    position: "absolute" as const,
    inset: "0",
    zIndex: -1,
    background: isColor ? background.value : undefined,
    backgroundImage: isColor
      ? undefined
      : dim.value
        ? `linear-gradient(#0005, #0008), url(${resolveAssetUrl(background.value)})`
        : `url("${resolveAssetUrl(background.value)}")`,
    backgroundRepeat: "no-repeat",
    backgroundPosition: "center",
    backgroundSize: "cover",
  };
});
</script>

<template>
  <div ref="el" :style="bgStyle" />
</template>
