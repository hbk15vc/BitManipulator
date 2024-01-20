/* Copyright 2018 Hobum (Vincent) Kwon. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <stack>
#include <cmath> 

#include "vtz_logger/common/logging.h"
#include "vtz_logger/common/macros.h"
#include "vtz_logger/common/integral_type.h"

namespace vtz {
namespace bit_manipulator {

std::string getHexaCharacter(char* buf, uint32_t startIndex, uint32_t size) {
  std::string s = "";
  int value = 0;
  for (int i = 0; i < size; i++) {
    value += (*(buf+startIndex+i) == '1' ? 1 : 0) * pow(2, size-i-1);
  }
  if (value >= 10) {
    char c = value + 55;
    s.push_back(c);
    //LOG(INFO) << "s.push result:" << s;
  } else {
    s.append(std::to_string(value));
    //LOG(INFO) << "s.push result:" << s;
  }
  //LOG(INFO) << s.c_str() ;
  return s;  
}

std::string getHexaString(char* value, int length) {
  std::string s;
  int numOfHexa = (length - 1) / 4 + 1;
  int reminderOfHexa = length % 4;
  int nextStartPoint = 0;

  if (reminderOfHexa > 0) {
    s += getHexaCharacter(value, 0, reminderOfHexa);
    numOfHexa -= 1;
    nextStartPoint = reminderOfHexa;
  }

  for (int i = 0; i < numOfHexa; i++) {
    s += getHexaCharacter(value, nextStartPoint + 4 * i, 4);
  }

  return s;
}

char* getBinaryString(int value) {
  char* returnStr = new char[32];
  std::stack<int> stack;

  if (value == 0) {
    returnStr[0] = '0';
    returnStr[1] = '\0';
    return returnStr;
  }

  for (int i = 0; i < 32; i++) {
  	int bitValue = value & 1;
  	stack.push(bitValue);
  	value = value >> 1;
  }

  while (stack.top() == 0) {
  	stack.pop();
  }

  int i = 0;
  while (!stack.empty()) {
  	int bitValue = stack.top();
  	if (bitValue) {
      returnStr[i] = '1';
  	} else {
      returnStr[i] = '0';
  	}
  	stack.pop();
  	i++;
  } 
  returnStr[i] = '\0';
  return returnStr;
}

int32 getBitValueFromIndexes(int startIndex, int endIndex, int32 originalValue) {
  if (startIndex < 0 || startIndex > endIndex || endIndex > 31) {
    //LOG(ERROR) << "wrong parameter.";
    return 0;
  } else {
    if (startIndex == endIndex) {
        return (originalValue & (1 << startIndex)); 
    } else {
      int32 maskInt = 0xFFFFFFFF;
      int32 lowerMaskBits = ((1 << (startIndex)) - 1);
      int32 upperMaskBits = 0;
      if (endIndex < 31) {
        upperMaskBits = maskInt - ((1 << (endIndex + 1)) - 1);
      }
      //LOG(INFO) << "mask bits: " << ~(upperMaskBits | lowerMaskBits);
      //LOG(INFO) << "mask bits in binary: " << getBinaryString(~(upperMaskBits | lowerMaskBits));
      int32 maskFinal = ~(upperMaskBits | lowerMaskBits);
      return (originalValue & maskFinal);
    }
  }
}

int32 setBitValueFromIndexes(int startIndex, int endIndex, int32 originalValue, int32 valueToSet) {
  if (startIndex < 0 || startIndex > endIndex || endIndex > 31) {
    //LOG(ERROR) << "wrong parameter.";
    return 0;
  } else {
    int32 lenOfNewValue = endIndex - startIndex + 1;
    if (lenOfNewValue < 32) {
      valueToSet = valueToSet & ((1 << lenOfNewValue) - 1);
    }
    
    if (startIndex == endIndex) {
       originalValue = (originalValue & ~(1 << startIndex));     
        // Set the bit now
        return (originalValue | (valueToSet << startIndex)); 
    } else {
      int32 maskInt = 0xFFFFFFFF;
      //int32 maskIntReverse = ~maskInt;
      int32 lowerMaskBits = ((1 << (startIndex)) - 1);
      int32 upperMaskBits = 0;
      if (endIndex < 31) {
        upperMaskBits = maskInt - ((1 << (endIndex + 1)) - 1);
      }
      
      originalValue &= (upperMaskBits | lowerMaskBits);  
      return originalValue | valueToSet << startIndex;
    }
  }
}


int32 setBitValueFromIndexes2(int startIndex, int endIndex, int32 originalValue, int startTargetIndex, int endTargetIndex, int32 valueToSet) {
  int32 valueToSetMasked = (getBitValueFromIndexes(startTargetIndex, endTargetIndex, valueToSet)) >> startTargetIndex;
  int32 temp = getBitValueFromIndexes(startTargetIndex, endTargetIndex, valueToSet);
  return setBitValueFromIndexes(startIndex, endIndex, originalValue, valueToSetMasked);
}

int getFirstNonZeroBitIndex(int32 value) {
  return __builtin_ctz(value);
}

int getFirstNonZeroBitValue(int32 value) {
  return (value & -value);
}

bool isPrime(int n) {
  return true;
}

void eratosthenes(int n) {

}

}  // bit_manipulator
}  // vtz
