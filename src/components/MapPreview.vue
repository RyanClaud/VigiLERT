<template>
  <div id="map-preview" class="h-40 w-full rounded overflow-hidden bg-gray-200"></div>
</template>

<script setup>
import L from 'leaflet';

const props = defineProps({
  startLat: Number,
  startLng: Number,
  endLat: Number,
  endLng: Number,
});

onMounted(() => {
  const map = L.map('map-preview').setView([props.startLat, props.startLng], 13);

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors',
  }).addTo(map);

  if (props.startLat && props.startLng) {
    L.marker([props.startLat, props.startLng]).addTo(map).bindPopup("Start Location");
  }
  if (props.endLat && props.endLng) {
    L.marker([props.endLat, props.endLng]).addTo(map).bindPopup("End Location");
  }

  if (props.startLat && props.startLng && props.endLat && props.endLng) {
    L.polyline([
      [props.startLat, props.startLng],
      [props.endLat, props.endLng],
    ], { color: '#3D52A0' }).addTo(map);

    map.fitBounds([
      [props.startLat, props.startLng],
      [props.endLat, props.endLng],
    ]);
  }
});
</script>