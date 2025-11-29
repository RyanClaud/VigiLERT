# PowerShell Script to Copy Logo Files
# Quick setup for testing - copies logo to PWA icon locations

Write-Host "🎨 Copying VigiLERT logo to PWA locations..." -ForegroundColor Cyan
Write-Host ""

$sourceLogo = "images\vigilert_logo.png"
$publicDir = "public"

# Check if source exists
if (-not (Test-Path $sourceLogo)) {
    Write-Host "❌ Error: vigilert_logo.png not found in images/ folder" -ForegroundColor Red
    exit 1
}

# Copy to PWA icon locations
Write-Host "📋 Copying logo files..." -ForegroundColor Yellow

Copy-Item $sourceLogo "$publicDir\pwa-192x192.png" -Force
Write-Host "✅ Copied to pwa-192x192.png" -ForegroundColor Green

Copy-Item $sourceLogo "$publicDir\pwa-512x512.png" -Force
Write-Host "✅ Copied to pwa-512x512.png" -ForegroundColor Green

# Copy to assets for use in app
if (-not (Test-Path "$publicDir\assets")) {
    New-Item -ItemType Directory -Path "$publicDir\assets" -Force | Out-Null
}

Copy-Item $sourceLogo "$publicDir\assets\vigilert_logo.png" -Force
Write-Host "✅ Copied to assets/vigilert_logo.png" -ForegroundColor Green

Write-Host ""
Write-Host "✨ Logo files copied successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "⚠️  Note: For favicon.ico, please use an online converter:" -ForegroundColor Yellow
Write-Host "   https://favicon.io/favicon-converter/" -ForegroundColor Cyan
Write-Host ""
Write-Host "📋 Next steps:" -ForegroundColor Yellow
Write-Host "   1. Generate favicon.ico from the logo" -ForegroundColor White
Write-Host "   2. Replace public/favicon.ico" -ForegroundColor White
Write-Host "   3. Clear browser cache and hard refresh" -ForegroundColor White
Write-Host ""
