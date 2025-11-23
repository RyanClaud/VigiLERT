<template>
  <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 border border-white/50">
    <div class="absolute top-0 left-0 w-64 h-64 bg-gradient-to-br from-blue-500/10 to-purple-500/10 rounded-full -ml-32 -mt-32"></div>
    
    <h3 class="relative text-2xl font-bold text-[#3D52A0] mb-6 flex items-center gap-3">
      <div class="bg-gradient-to-br from-blue-500 to-blue-600 p-3 rounded-2xl shadow-lg">
        <span class="material-icons text-3xl text-white">sensors</span>
      </div>
      <span>Sensor Health Monitor</span>
      <div class="ml-auto flex items-center gap-2">
        <span class="w-2 h-2 bg-green-500 rounded-full animate-pulse"></span>
        <span class="text-sm font-normal text-gray-600">Live</span>
      </div>
    </h3>

    <div class="relative grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      <!-- MPU6050 Sensor -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        getSensorStatus('mpu6050').isActive ? 'bg-green-50 border-green-500' : 'bg-gray-50 border-gray-300']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', getSensorStatus('mpu6050').isActive ? 'bg-green-500' : 'bg-gray-400']">
              <span class="material-icons text-white text-2xl">rotate_90_degrees_ccw</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">MPU6050</h4>
              <p class="text-xs text-gray-500">Gyroscope/Accelerometer</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', getSensorStatus('mpu6050').isActive ? 'bg-green-500 animate-pulse' : 'bg-gray-400']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Status:</span>
            <span :class="['font-semibold', getSensorStatus('mpu6050').isActive ? 'text-green-600' : 'text-gray-500']">
              {{ getSensorStatus('mpu6050').status }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Last Update:</span>
            <span class="font-mono text-xs text-gray-700">{{ getSensorStatus('mpu6050').lastUpdate }}</span>
          </div>
          <div v-if="sensorData.mpu6050" class="mt-3 pt-3 border-t border-gray-200">
            <div class="grid grid-cols-2 gap-2 text-xs">
              <div>
                <span class="text-gray-500">Accel X:</span>
                <span class="font-semibold ml-1">{{ sensorData.mpu6050.accelX || 0 }}</span>
              </div>
              <div>
                <span class="text-gray-500">Accel Y:</span>
                <span class="font-semibold ml-1">{{ sensorData.mpu6050.accelY || 0 }}</span>
              </div>
              <div>
                <span class="text-gray-500">Accel Z:</span>
                <span class="font-semibold ml-1">{{ sensorData.mpu6050.accelZ || 0 }}</span>
              </div>
              <div>
                <span class="text-gray-500">Gyro:</span>
                <span class="font-semibold ml-1">{{ sensorData.mpu6050.gyro || 0 }}°</span>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- GPS Module -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        gpsConnected ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', gpsConnected ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white text-2xl">gps_fixed</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">GPS Module</h4>
              <p class="text-xs text-gray-500">Location Tracking</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', gpsConnected ? 'bg-green-500 animate-pulse' : 'bg-red-500']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Status:</span>
            <span :class="['font-semibold', gpsConnected ? 'text-green-600' : 'text-red-600']">
              {{ gpsConnected ? 'Active' : 'Offline' }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Accuracy:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.gps?.accuracy || 'N/A' }}</span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Satellites:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.gps?.satellites || '0' }}</span>
          </div>
        </div>
      </div>

      <!-- GSM Module -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        gsmConnected ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', gsmConnected ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white text-2xl">signal_cellular_alt</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">GSM Module</h4>
              <p class="text-xs text-gray-500">Cellular Connection</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', gsmConnected ? 'bg-green-500 animate-pulse' : 'bg-red-500']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Status:</span>
            <span :class="['font-semibold', gsmConnected ? 'text-green-600' : 'text-red-600']">
              {{ gsmConnected ? 'Connected' : 'Offline' }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Signal:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.gsm?.signal || '0%' }}</span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Network:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.gsm?.network || 'N/A' }}</span>
          </div>
        </div>
      </div>

      <!-- Engine Status -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        sensorData.engineRunning ? 'bg-green-50 border-green-500' : 'bg-gray-50 border-gray-300']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', sensorData.engineRunning ? 'bg-green-500 animate-pulse' : 'bg-gray-400']">
              <span class="material-icons text-white text-2xl">power_settings_new</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">Engine Status</h4>
              <p class="text-xs text-gray-500">Relay Control Monitor</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', sensorData.engineRunning ? 'bg-green-500 animate-pulse' : 'bg-gray-400']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Status:</span>
            <span :class="['font-semibold', sensorData.engineRunning ? 'text-green-600' : 'text-gray-500']">
              {{ sensorData.engineRunning ? 'Running' : 'Stopped' }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Relay:</span>
            <span :class="['font-bold text-xl', sensorData.engineRunning ? 'text-green-600' : 'text-gray-500']">
              {{ sensorData.engineRunning ? 'ON' : 'OFF' }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Condition:</span>
            <span :class="['font-semibold', sensorData.engineRunning ? 'text-green-600' : 'text-gray-500']">
              {{ sensorData.engineRunning ? 'Active' : 'Inactive' }}
            </span>
          </div>
        </div>
      </div>

      <!-- Alcohol Sensor -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        getSensorStatus('alcohol').isActive ? 'bg-green-50 border-green-500' : 'bg-gray-50 border-gray-300']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', getSensorStatus('alcohol').isActive ? 'bg-green-500' : 'bg-gray-400']">
              <span class="material-icons text-white text-2xl">local_drink</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">Alcohol Sensor</h4>
              <p class="text-xs text-gray-500">MQ-3 Gas Sensor</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', getSensorStatus('alcohol').isActive ? 'bg-green-500 animate-pulse' : 'bg-gray-400']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Status:</span>
            <span :class="['font-semibold', getSensorStatus('alcohol').isActive ? 'text-green-600' : 'text-gray-500']">
              {{ getSensorStatus('alcohol').status }}
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Value:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.alcohol?.value || '0' }} ppm</span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Safety:</span>
            <span :class="['font-semibold', alcoholStatus === 'Safe' ? 'text-green-600' : 'text-red-600']">
              {{ alcoholStatus }}
            </span>
          </div>
        </div>
      </div>

      <!-- Battery Monitor -->
      <div :class="['p-5 rounded-2xl border-2 transition-all duration-300 hover:scale-105', 
        deviceBattery > 20 ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
        <div class="flex items-start justify-between mb-3">
          <div class="flex items-center gap-3">
            <div :class="['p-2 rounded-xl', deviceBattery > 20 ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white text-2xl">battery_charging_full</span>
            </div>
            <div>
              <h4 class="font-bold text-gray-800">Battery</h4>
              <p class="text-xs text-gray-500">Power Management</p>
            </div>
          </div>
          <div :class="['w-3 h-3 rounded-full', deviceBattery > 20 ? 'bg-green-500 animate-pulse' : 'bg-red-500 animate-pulse']"></div>
        </div>
        <div class="space-y-2">
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Level:</span>
            <span :class="['font-bold text-xl', deviceBattery > 20 ? 'text-green-600' : 'text-red-600']">
              {{ deviceBattery }}%
            </span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Voltage:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.battery?.voltage || '12.6V' }}</span>
          </div>
          <div class="flex justify-between text-sm">
            <span class="text-gray-600">Health:</span>
            <span class="font-semibold text-gray-700">{{ sensorData.battery?.health || '95%' }}</span>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue';

const props = defineProps({
  gpsConnected: Boolean,
  gsmConnected: Boolean,
  deviceBattery: Number,
  alcoholStatus: String,
  sensorData: {
    type: Object,
    default: () => ({})
  }
});

const getSensorStatus = (sensor) => {
  const now = Date.now();
  const lastUpdate = props.sensorData[sensor]?.lastUpdate || 0;
  const timeDiff = now - lastUpdate;
  
  const isActive = timeDiff < 10000; // Active if updated within 10 seconds
  
  return {
    isActive,
    status: isActive ? 'Active' : 'Offline',
    lastUpdate: isActive ? `${Math.floor(timeDiff / 1000)}s ago` : 'No data'
  };
};

// ✅ Heart rate functions removed - replaced with Engine Status
// Engine status is now directly read from sensorData.engineRunning
</script>

<style scoped>
.hover\:scale-105:hover {
  transform: scale(1.05);
}
</style>
