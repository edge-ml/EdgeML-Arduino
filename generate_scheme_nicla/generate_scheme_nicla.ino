
#include "scheme_generator.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {};
  Serial.println("START"); 

  schemeData * data;
  String code;
  data = make_scheme_buffer();

  code = make_code(data);

  Serial.println(code);

  Serial.println("END");
}

void loop() {
  
}
