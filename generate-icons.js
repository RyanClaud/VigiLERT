/**
 * Icon Generator Script
 * Generates PWA icons and favicon from vigilert_logo.png
 * 
 * Usage: node generate-icons.js
 * 
 * Requirements: npm install sharp --save-dev
 */

const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

const INPUT_IMAGE = path.join(__dirname, 'images', 'vigilert_logo.png');
const OUTPUT_DIR = path.join(__dirname, 'public');

// Icon sizes to generate
const ICON_SIZES = [
  { size: 16, name: 'favicon-16x16.png' },
  { size: 32, name: 'favicon-32x32.png' },
  { size: 192, name: 'pwa-192x192.png' },
  { size: 512, name: 'pwa-512x512.png' },
  { size: 180, name: 'apple-touch-icon.png' }, // For iOS
];

async function generateIcons() {
  console.log('🎨 Starting icon generation...\n');

  // Check if input image exists
  if (!fs.existsSync(INPUT_IMAGE)) {
    console.error('❌ Error: vigilert_logo.png not found in images/ folder');
    process.exit(1);
  }

  console.log(`📁 Input: ${INPUT_IMAGE}`);
  console.log(`📁 Output: ${OUTPUT_DIR}\n`);

  try {
    // Generate each icon size
    for (const { size, name } of ICON_SIZES) {
      const outputPath = path.join(OUTPUT_DIR, name);
      
      await sharp(INPUT_IMAGE)
        .resize(size, size, {
          fit: 'contain',
          background: { r: 255, g: 255, b: 255, alpha: 0 } // Transparent background
        })
        .png()
        .toFile(outputPath);
      
      console.log(`✅ Generated: ${name} (${size}x${size})`);
    }

    // Generate favicon.ico (multi-size)
    console.log('\n🔄 Generating favicon.ico...');
    
    // For favicon.ico, we'll use the 32x32 version
    // Note: Sharp doesn't support .ico format directly
    // You'll need to use an online converter or ImageMagick for true .ico
    const faviconPath = path.join(OUTPUT_DIR, 'favicon.png');
    await sharp(INPUT_IMAGE)
      .resize(32, 32, {
        fit: 'contain',
        background: { r: 255, g: 255, b: 255, alpha: 0 }
      })
      .png()
      .toFile(faviconPath);
    
    console.log('✅ Generated: favicon.png (32x32)');
    console.log('\n⚠️  Note: favicon.png created. For true .ico format, use:');
    console.log('   - https://favicon.io/favicon-converter/');
    console.log('   - Or ImageMagick: magick favicon.png favicon.ico');

    console.log('\n✨ Icon generation complete!');
    console.log('\n📋 Next steps:');
    console.log('   1. Convert favicon.png to favicon.ico using online tool');
    console.log('   2. Clear browser cache (Ctrl + Shift + Delete)');
    console.log('   3. Hard refresh (Ctrl + F5)');
    console.log('   4. Test PWA installation');

  } catch (error) {
    console.error('❌ Error generating icons:', error.message);
    process.exit(1);
  }
}

// Run the generator
generateIcons();
