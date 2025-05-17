const admin = require("firebase-admin");

// Initialize Firebase Admin SDK
admin.initializeApp({
  credential: admin.credential.cert(require("./vigilance-shield-firebase-adminsdk-fbsvc-f2a16d394f.json")),
  databaseURL: "https://vigilance-shield-default-rtdb.firebaseio.com"
});

// Generate a custom token for the Arduino device
const uid = "MnzBjTBslZNijOkq732PE91hHa23"; //UID
admin.auth().createCustomToken(uid)
  .then((customToken) => {
    console.log("Custom Token:", customToken);
    // Send this token to the Arduino device or store it securely
  })
  .catch((error) => {
    console.error("Error creating custom token:", error);
  });