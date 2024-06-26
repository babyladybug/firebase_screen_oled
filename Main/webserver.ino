//webpage hosted on the server
char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
  <form>
    <fieldset>
      <div>
        <label for="URL">Enter txt URL</label>
        <input type="text" value="" id="URL" placeholder="URL">
      </div>
      <div>
        <button class="primary" id="savebtn" type="button" onclick="myFunction()">SAVE</button>
      </div>
    </fieldset>
  </form>
</body>
<script>

function myFunction()
{
  console.log("button was clicked!");
  var link = document.getElementById("URL").value;
  var data = {link:link};
  var xhr = new XMLHttpRequest();
  var url = "/settings";
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Typical action to be performed when the document is ready:
      if(xhr.responseText != null){
        console.log(xhr.responseText);
        document.getElementById("savebtn").style.backgroundColor = "green";
      }
    }
  };
  xhr.open("POST", url, true);
  xhr.send(JSON.stringify(data));
};
</script>
</html>
)=====";

//Hosting the server on esp
void startServer()
{ 
  server.on("/",[](){server.send_P(200,"text/html", webpage);});
  server.on("/settings", HTTP_POST, handleSettingUpdate);
  server.begin();
}

//this function gets the URL and stores in the flash memory
void handleSettingUpdate() {
  String data = server.arg("plain");  
  
  // Define a fixed capacity for the JSON document, adjust as needed
  const size_t capacity = 1024;
  
  // JSON buffer to store the JSON
  DynamicJsonDocument doc(capacity);
  
  deserializeJson(doc, data);
  
  File configFile = SPIFFS.open("/config.json", "w");
  
  serializeJson(doc, configFile);
  serializeJson(doc, Serial);
  configFile.close();
  
  server.send(200, "application/json", "{\"status\":\"ok\"}");
  
  delay(500);
  
  readFlashJson();
  SPIFFS.remove(textFilePath);
  textFile = "";
  currentScreen = MENU;
  updateScreen = true;
}
