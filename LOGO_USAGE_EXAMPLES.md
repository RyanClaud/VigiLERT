# VigiLERT Logo Usage Examples

## Files Updated
✅ `public/pwa-192x192.png` - PWA icon (192x192)
✅ `public/pwa-512x512.png` - PWA icon (512x512)
✅ `public/logo.png` - Logo for use in app
✅ `index.html` - Added Apple touch icon reference

## Using Logo in Vue Components

### Example 1: Login Page Header
```vue
<template>
  <div class="login-header">
    <img src="/logo.png" alt="VigiLERT" class="logo" />
    <h1>Welcome to VigiLERT</h1>
  </div>
</template>

<style scoped>
.logo {
  width: 120px;
  height: auto;
  margin-bottom: 20px;
}
</style>
```

### Example 2: Dashboard Header
```vue
<template>
  <header class="dashboard-header">
    <div class="logo-container">
      <img src="/logo.png" alt="VigiLERT" class="logo" />
      <span class="brand-name">VigiLERT</span>
    </div>
  </header>
</template>

<style scoped>
.logo-container {
  display: flex;
  align-items: center;
  gap: 12px;
}

.logo {
  width: 40px;
  height: 40px;
}

.brand-name {
  font-size: 24px;
  font-weight: bold;
  color: #3D52A0;
}
</style>
```

### Example 3: Centered Logo (Loading Screen)
```vue
<template>
  <div class="loading-screen">
    <img src="/logo.png" alt="VigiLERT" class="logo-large" />
    <p>Loading...</p>
  </div>
</template>

<style scoped>
.loading-screen {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 100vh;
  background: linear-gradient(to bottom, #EDE8F5, #f5f3f7);
}

.logo-large {
  width: 200px;
  height: auto;
  animation: pulse 2s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% {
    opacity: 1;
    transform: scale(1);
  }
  50% {
    opacity: 0.8;
    transform: scale(1.05);
  }
}
</style>
```

### Example 4: Navigation Bar Logo
```vue
<template>
  <nav class="navbar">
    <div class="nav-brand">
      <img src="/logo.png" alt="VigiLERT" class="nav-logo" />
      <span>VigiLERT</span>
    </div>
    <div class="nav-links">
      <!-- Navigation links -->
    </div>
  </nav>
</template>

<style scoped>
.navbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px 32px;
  background: white;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.nav-brand {
  display: flex;
  align-items: center;
  gap: 12px;
  font-size: 20px;
  font-weight: bold;
  color: #3D52A0;
}

.nav-logo {
  width: 36px;
  height: 36px;
}
</style>
```

## Favicon Update (Important!)

The favicon.ico still needs to be updated. Here's how:

### Option 1: Online Converter (Recommended)
1. Go to https://favicon.io/favicon-converter/
2. Upload `images/vigilert_logo.png`
3. Download the generated `favicon.ico`
4. Replace `public/favicon.ico` with the new file

### Option 2: Using ImageMagick
```bash
magick images/vigilert_logo.png -define icon:auto-resize=16,32,48 public/favicon.ico
```

## Testing Your Changes

1. **Clear Browser Cache**
   - Chrome: Ctrl + Shift + Delete
   - Select "Cached images and files"
   - Click "Clear data"

2. **Hard Refresh**
   - Windows: Ctrl + F5
   - Mac: Cmd + Shift + R

3. **Check PWA Icons**
   - Open DevTools (F12)
   - Go to Application tab
   - Click "Manifest" in sidebar
   - Verify icons are loading

4. **Test PWA Installation**
   - Click install button in browser
   - Check if app icon shows your logo

## PWA Manifest (Already Configured)

Your `manifest.webmanifest` is already set up correctly:
```json
{
  "name": "VigiLERT",
  "short_name": "VigiLERT",
  "icons": [
    {
      "src": "pwa-192x192.png",
      "sizes": "192x192",
      "type": "image/png"
    },
    {
      "src": "pwa-512x512.png",
      "sizes": "512x512",
      "type": "image/png",
      "purpose": "any maskable"
    }
  ]
}
```

## Responsive Logo Sizes

For different screen sizes:
```css
/* Mobile */
@media (max-width: 768px) {
  .logo {
    width: 32px;
    height: 32px;
  }
}

/* Tablet */
@media (min-width: 769px) and (max-width: 1024px) {
  .logo {
    width: 40px;
    height: 40px;
  }
}

/* Desktop */
@media (min-width: 1025px) {
  .logo {
    width: 48px;
    height: 48px;
  }
}
```

## Next Steps

1. ✅ PWA icons updated (192x192, 512x512)
2. ✅ Logo available at `/logo.png`
3. ✅ Apple touch icon configured
4. ⚠️ **TODO**: Update favicon.ico using online converter
5. ⚠️ **TODO**: Add logo to Login/Register pages
6. ⚠️ **TODO**: Add logo to Dashboard header (optional)

## Quick Test

After updating favicon.ico, test by:
1. Opening your app in a new incognito window
2. Checking the browser tab icon
3. Installing as PWA and checking the app icon on desktop/home screen
