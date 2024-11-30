/*
  When I'm writing this code, only I and God know what it does. Now, only God knows.
                                                                  -- Hieu --
*/


// Zone library
#include <WiFi.h>
#include <Stepper.h>
#include <ESP32Servo.h>
#include <ArduinoQueue.h>
#include "Arduino.h"
#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems
#include "esp_http_server.h"

// Create queue
struct commands{
  String command;
   int value;
};
ArduinoQueue<commands> command_queue(800);  // lưu các tập lệnh


// left 35 - 32 - 33 - 25
// right 26 - 27 - 14 - 12

// Replace with your network credentials
const char* ssid = "Viet Dung T1";
const char* password = "054850208";

#define PART_BOUNDARY "123456789000000000000987654321"


// Stepper Motor Setup
const int stepsPerRevolution = 2048;

// Initialize Stepper instances for two motors
// Motor 1 connected to pins 8, 10, 9, 11
Stepper myStepper1(stepsPerRevolution, 35, 32, 33, 25);
//
//// Motor 2 connected to pins 4, 6, 5, 7
Stepper myStepper2(stepsPerRevolution, 26, 27, 14, 12);
//
//// Servo Setup
//Servo servo_9;


float wheel_dia=63; //    # mm (increase = spiral out)
float wheel_base=109; //    # mm (increase = spiral in, ccw) 
int steps_rev=128; //        # 512 for 64x gearbox, 128 for 16x gearbox
int delay_time=6; //         # time between steps in ms



// setup servo
int servoPin = 8;
int PEN_DOWN = 20; // angle of servo when pen is down
int PEN_UP = 80;   // angle of servo when pen is up
Servo penServo;


// Websever zone

httpd_handle_t camera_httpd = NULL;
static const char PROGMEM INDEX_HTML[] = R"rawliteral(

<html>
   <head>
      <title>JavaScript Turtle Graphics</title>
      <meta charset="utf-8">
      <style>
         /* Bố cục Flexbox cho giao diện */
         body {
            margin: 0;
            font-family: Arial, sans-serif;
            display: flex;
            height: 100vh;
         }

         /* Phần bên trái chứa textarea và nút */
         #left-side {
            flex: 1;
            display: flex;
            flex-direction: column;
            background-color: #333;
            color: #fff;
            padding: 10px;
         }

         /* Phần nút "Run Code" */
         #controls {
            display: flex;
            justify-content: space-between;
            margin-bottom: 10px;
         }

         /* Textarea để nhập mã */
         #definitions {
            flex: 1;
            width: 100%;
            height: 100%;
            background-color: #2b2b2b;
            color: #fff;
            font-family: monospace;
            font-size: 14px;
            padding: 10px;
            border: none;
         }

         /* Phần bên phải chứa canvas */
         #right-side {
            flex: 1;
            padding: 10px;
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: #f0f0f0;
         }

         /* Canvas tùy chỉnh */
         #turtlecanvas {
            width: 100%;
            height: 100%;
            max-width: 700px;
            max-height: 700px;
            border: 1px solid red;
         }

         /* Nút "Run Code" và "Reset" */
         button {
            padding: 5px 15px;
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
         }

         button:hover {
            background-color: #45a049;
         }
      </style>
   </head>
   <body>
      <!-- Phần bên trái: Textarea và nút "Run Code" -->
      <div id="left-side">
         <div id="controls">
            <h2>Definitions</h2>
            <button onclick="runIt()">Run Code</button>
         </div>
         <textarea id="definitions">// Nhập mã của bạn ở đây</textarea>
      </div>

      <!-- Phần bên phải: Canvas -->
      <div id="right-side">
            <canvas id="turtlecanvas" width="700" height="700" ></canvas>
            <canvas id="imagecanvas" width="700" height="700" style="display:none"></canvas>

      </div>

      <!-- Nhúng các file JavaScript -->
                                          
      <script type="text/javascript" src="http://127.0.0.1:5500/js-turtle-master/lib/library.js"></script>
      <script type="text/javascript" src="http://127.0.0.1:5500/js-turtle-master/functions.js"></script>
      <script type="text/javascript" src="http://127.0.0.1:5500/js-turtle-master/turtle.js"></script>

      <script>
         function runIt() {
            // Lấy mã từ textarea
            const code = document.getElementById('definitions').value;
            try {
               eval(code); // Chạy mã nhập vào
            } catch (error) {
               alert("Lỗi: " + error.message);
            }
         }
      </script>
   </body>
</html>
)rawliteral";

// Zone for control 


int step(float distance){
  int steps = distance * steps_rev / (wheel_dia * 3.1412); //24.61
  /*
  Serial.print(distance);
  Serial.print(" ");
  Serial.print(steps_rev);
  Serial.print(" ");  
  Serial.print(wheel_dia);
  Serial.print(" ");  
  Serial.println(steps);
  delay(1000);*/
  return steps;  
}


void forward(float distance){
  int steps = step(distance);
  Serial.println(steps);
  for(int step=0; step<steps; step++){
     myStepper1.step(-1); 
     myStepper2.step(1);
     delay(delay_time);
  }
}


void backward(float distance){
    int steps = step(distance);
  Serial.println(steps);
  for(int step=0; step<steps; step++){
     myStepper1.step(1); 
     myStepper2.step(-1);
     delay(delay_time);
  }
}


void right(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    myStepper1.step(-1); 
    myStepper2.step(-1);
    delay(delay_time);
  }
}


void left(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
     myStepper1.step(1); 
     myStepper2.step(1);
     delay(delay_time);
  }
}




void penup(){
  delay(250);
  Serial.println("PEN_UP()");
  penServo.write(PEN_UP);
  delay(250);
}


void pendown(){
  delay(250);  
  Serial.println("PEN_DOWN()");
  penServo.write(PEN_DOWN);
  delay(250);
}

void runIt(){
    while(!command_queue.isEmpty()){
       commands tmp = command_queue.dequeue();
       if(tmp.command == "finished") break;
       if(tmp.command == "foward"){
          forward(tmp.value);
       } else if(tmp.command == "backward"){
         backward(tmp.value);
       } else if(tmp.command == "right"){
         right(tmp.value);
       } else if(tmp.command == "left"){
         left(tmp.value);
       } else if(tmp.command == "pen"){
         if(tmp.value == 1) penup(); else pendown();
       }
    }
}



static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t cmd_handler(httpd_req_t *req) {
    char* buf;
    size_t buf_len;
    char variable[32] = {0};
    bool ok = false;
    char tmp[32] = {};
    String value = "";
    
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if (!buf) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
                for (int len = 0; len < strlen(variable) && len < sizeof(variable); len++) {
                    if (variable[len] == '(') {
                        ok = true;
                        tmp[len] = '\0';
                        continue;
                    }
                    if (variable[len] == ')') {
                        break;
                    }
                    if (!ok) {
                        tmp[len] = variable[len];
                    } else {
                        value += variable[len];
                    }
                }
                free(buf);

                // Xử lý lệnh và giá trị
                if (strcmp(tmp, "forward") == 0) {
                    Serial.println("Command: Forward");
                    Serial.print("Value: ");
                    Serial.println(value);
                    commands t("forward" , value.toInt());
                    command_queue.enqueue(t);
                } else if (strcmp(tmp, "backward") == 0) {
                    Serial.println("Command: Backward");
                    Serial.print("Value: ");
                    Serial.println(value);
                    commands t("backward" , value.toInt());
                    command_queue.enqueue(t);
                } else if (strcmp(tmp, "left") == 0) {
                    Serial.println("Command: Left");
                    Serial.print("Value: ");
                    Serial.println(value);
                    commands t("left" , value.toInt());
                    command_queue.enqueue(t);
                }  else if (strcmp(tmp, "right") == 0) {
                    Serial.println("Command: Right");
                    Serial.print("Value: ");
                    Serial.println(value);
                    commands t("right" , value.toInt());
                    command_queue.enqueue(t);
                }  else if (strcmp(tmp, "pen") == 0) {
                    Serial.println("Command: Pen");
                    Serial.print("Value: ");
                    Serial.println(value);
                    commands t("pen" , value.toInt());
                    command_queue.enqueue(t);
                }  else if(strcmp(tmp,"finished") == 0){
                    runIt();
                }
                return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
            } else {
                free(buf);
                httpd_resp_send_404(req);
                return ESP_FAIL;
            }
        } else {
            free(buf);
            httpd_resp_send_404(req);
            return ESP_FAIL;
        }
    } else {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }
}

void startServer(){
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = index_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t cmd_uri = {
        .uri       = "/action",
        .method    = HTTP_GET,
        .handler   = cmd_handler,
        .user_ctx  = NULL
    };

    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &cmd_uri);
    } else {
        Serial.println("Failed to start server!");
    }
}


void setup() {

  
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
  
  myStepper1.setSpeed(10);
  myStepper2.setSpeed(10);

  for (int i = 0; i < stepsPerRevolution; i++) {
    myStepper1.step(1);  // Step motor 1 one step
    myStepper2.step(-1); // Step motor 2 one step (opposite direction for symmetric setup)
  }

  
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  
  // Start streaming web server
  startServer();

  
}

void loop() {
  
}