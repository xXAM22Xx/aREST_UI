/*
  aREST UI for Arduino & the ESP8266
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
  Version 1.1.1

  Changelog:

  Version 1.1.1: Fixed bug with NodeMCU boards
  Version 1.1.0: Added support for functions
  Version 1.0.1: Initial release with buttons only
*/

#ifndef aRest_ui_h
#define aRest_ui_h

// Include Arduino header
#include "Arduino.h"

// Using ESP8266 ?
#if defined(ESP8266)
#include "stdlib_noniso.h"
#endif

class aREST_UI: public aREST {

public:

aREST_UI() {

}

// Get title
void title(String the_title) {
  ui_title = the_title;
}

// Create button
void button(int pin,char* name){

  // Set pin as output
  #if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
    pinMode(esp_12_pin_map(pin), OUTPUT);
  #else
    pinMode(pin, OUTPUT);
  #endif

  // Set in button array
  buttons[buttons_index] = pin;
  buttons_names[buttons_index] = name;
  buttons_index++;

  uiElements[uiElements_index] = uiButton;
  uiElements_index++;
}

// Create function control
void callFunction(char * functionName, char * type){

  // Set in callFunction array
  callFunctionNames[functions_index] = functionName;
  callFunctionTypes[functions_index] = type;
  functions_index++;

  uiElements[uiElements_index] = uiFunction;
  uiElements_index++;
}

// Create slider
void slider(int pin) {

  // Set pin as output
  #if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
    pinMode(esp_12_pin_map(pin), OUTPUT);
  #else
    pinMode(pin, OUTPUT);
  #endif

  // Set in button array
  sliders[sliders_index] = pin;
  sliders_index++;

  uiElements[uiElements_index] = uiSlider;
  uiElements_index++;
}

// Create label
void label(char * label_name){

  int_labels_names[int_labels_index] = label_name;
  int_labels_index++;

  uiElements[uiElements_index] = uiLabel;
  uiElements_index++;
}

void loadUI(void) {
	int num_buttons = 0;
	int num_functions = 0;
	int num_slider = 0;
	int num_label = 0;

	for (int i = 0; i < uiElements_index; i++) {

		switch (uiElements[i])
		{
		case uiButton: addUiButton(num_buttons); num_buttons++;
			break;
		case uiFunction: addUiFunction(num_functions); num_functions++;
			break;
		case uiSlider: addUiSlider(num_slider); num_slider++;
			break;
		case uiLabel: addUiLabel(num_label); num_label++;
			break;
		default:
			break;
		}
	}
}

void addUiButton(int id) {
	addToBuffer("<div class=\"row\">");
	addToBuffer("");
	addToBuffer("<div class=\"col-md-2\"><button class=\"btn btn-block btn-lg btn-primary\" id='btn_on");
	addToBuffer(buttons[id]);
	addToBuffer("'>");
	addToBuffer(buttons_names[id]);
	addToBuffer(" (On)</button></div>");
	addToBuffer("<div class=\"col-md-2\"><button class=\"btn btn-block btn-lg btn-danger\" id='btn_off");
	addToBuffer(buttons[id]);
	addToBuffer("'>");
	addToBuffer(buttons_names[id]);
	addToBuffer(" (Off)</button></div>");
	addToBuffer("</div>");
}

void addUiFunction(int id) {
	if (callFunctionTypes[id] == "push") {

		addToBuffer("<div class=\"row\">");
		addToBuffer("<div class=\"col-md-3\"><button class=\"btn btn-block btn-lg btn-primary\" id='fn_push_");
		addToBuffer(callFunctionNames[id]);
		addToBuffer("'>");
		addToBuffer(callFunctionNames[id]);
		addToBuffer("</button></div>");
		addToBuffer("</div>");

	}
}

void addUiSlider(int id) {
	addToBuffer("<div class=\"row\">");
	addToBuffer("<div class=\"col-md-2\"><input type='range' value='0' max='255' min='0' step='5' id='slider");
	addToBuffer(sliders[id]);
	addToBuffer("'></div>");
	addToBuffer("</div>");
}

void addUiLabel(int id) {
	addToBuffer("<div class=\"row\">");
	addToBuffer("<div class='col-md-3 indicator'>");
	addToBuffer(int_labels_names[id]);
	addToBuffer(": </div>");
	addToBuffer("<div class='col-md-3 indicator' id='");
	addToBuffer(int_labels_names[id]);
	addToBuffer("'></div>");
	addToBuffer("</div>");
}

// Handle connection
virtual void root_answer() {

    // Answer
    addToBuffer("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    addToBuffer("<html><head>");
    addToBuffer("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    addToBuffer("<script ");
    addToBuffer("src=\"http://code.jquery.com/jquery-2.1.3.min.js\">");
    addToBuffer("</script>");
    addToBuffer("<script type='text/javascript' src='http://cdn.rawgit.com/Foliotek/AjaxQ/master/ajaxq.js'></script>");
    addToBuffer("<style>.row {margin-top: 30px;} .indicator {font-size: 30px; vertical-align: middle;}</style>");
    addToBuffer("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">");
    addToBuffer("</head><body>");
    addToBuffer("<div class=\"container\">");

    // Title
    if (ui_title.length() != 0) {
      addToBuffer("<h1>");
      addToBuffer(ui_title);
      addToBuffer("</h1>");
    }
    else {
      addToBuffer("<h1>Interface</h1>");
    }

	loadUI();
    //// Buttons UI
    //for (int i = 0; i < buttons_index; i++) {
    //  addToBuffer("<div class=\"row\">");
    //  addToBuffer("<div class=\"col-md-2\"><button class=\"btn btn-block btn-lg btn-primary\" id='btn_on");
    //  addToBuffer(buttons[i]);
    //  addToBuffer("'>On</button></div>");
    //  addToBuffer("<div class=\"col-md-2\"><button class=\"btn btn-block btn-lg btn-danger\" id='btn_off");
    //  addToBuffer(buttons[i]);
    //  addToBuffer("'>Off</button></div>");
    //  addToBuffer("</div>");
    //}

    //// Function calls UI
    //for (int i = 0; i < functions_index; i++) {

    //  if (callFunctionTypes[i] == "push") {

    //    addToBuffer("<div class=\"row\">");
    //    addToBuffer("<div class=\"col-md-3\"><button class=\"btn btn-block btn-lg btn-primary\" id='fn_push_");
    //    addToBuffer(callFunctionNames[i]);
    //    addToBuffer("'>");
    //    addToBuffer(callFunctionNames[i]);
    //    addToBuffer("</button></div>");
    //    addToBuffer("</div>");

    //  }
    //}

    //// Sliders UI
    //for (int i = 0; i < sliders_index; i++) {
    //  addToBuffer("<div class=\"row\">");
    //  addToBuffer("<div class=\"col-md-2\"><input type='range' value='0' max='255' min='0' step='5' id='slider");
    //  addToBuffer(sliders[i]);
    //  addToBuffer("'></div>");
    //  addToBuffer("</div>");
    //}

    //// Labels UI
    //for (int j = 0; j < int_labels_index; j++) {
    //  addToBuffer("<div class=\"row\">");
    //  addToBuffer("<div class='col-md-3 indicator'>");
    //  addToBuffer(int_labels_names[j]);
    //  addToBuffer(": </div>");
    //  addToBuffer("<div class='col-md-3 indicator' id='");
    //  addToBuffer(int_labels_names[j]);
    //  addToBuffer("'></div>");
    //  addToBuffer("</div>");
    //}

    addToBuffer("</div>");

    addToBuffer("<script>$( document ).ready(function() {");

    // Buttons JavaScript
    for (int i = 0; i < buttons_index; i++) {
      addToBuffer("$('#btn_on");
      addToBuffer(buttons[i]);
      addToBuffer("').click(function() {$.getq('queue','/digital/");
      addToBuffer(buttons[i]);
      addToBuffer("/1');});");
      addToBuffer("$('#btn_off");
      addToBuffer(buttons[i]);
      addToBuffer("').click(function() {$.getq('queue','/digital/");
      addToBuffer(buttons[i]);
      addToBuffer("/0');});");
    }

    // Functions JavaScript
    for (int i = 0; i < functions_index; i++) {
      addToBuffer("$('#fn_push_");
      addToBuffer(callFunctionNames[i]);
      addToBuffer("').click(function() {$.getq('queue','/");
      addToBuffer(callFunctionNames[i]);
      addToBuffer("');location.reload();});");
    }

    // Sliders JavaScript
    for (int i = 0; i < sliders_index; i++) {
      addToBuffer("$('#slider");
      addToBuffer(sliders[i]);
      addToBuffer("').mouseup(function() {var val = $('#slider");
      addToBuffer(sliders[i]);
      addToBuffer("').val(); $.getq('queue','/analog/");
      addToBuffer(sliders[i]);
      addToBuffer("/' + val); });");
    }

    // Labels JavaScript
    for (int j = 0; j < int_labels_index; j++) {
      addToBuffer("$.getq('queue','/");
      addToBuffer(int_labels_names[j]);
      addToBuffer("', function(data) { $('#");
      addToBuffer(int_labels_names[j]);
      addToBuffer("').html(data.");
      addToBuffer(int_labels_names[j]);
      addToBuffer("); });");
    }

    addToBuffer("});</script>");

    addToBuffer("</body></html>\r\n");

}

private:

  // UI title
  String ui_title;

  // Buttons array
  int buttons[15];
  char * buttons_names[15];
  int buttons_index;

  // Functions array
  char * callFunctionNames[15];
  char * callFunctionTypes[15];
  int functions_index;

  // Buttons array
  int sliders[15];
  int sliders_index;

  // Indicators array
  uint8_t int_labels_index;
  int * int_labels_variables[15];
  char * int_labels_names[15];

  // UI Array
  int uiElements[25];
  int uiElements_index;
  enum uiElement { uiButton,uiFunction,uiSlider,uiLabel };

};

#endif
