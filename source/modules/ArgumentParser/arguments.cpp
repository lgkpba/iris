/*
 * Copyright 2021 João F. BeyondMagic <koetemagie@gmail.com>
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 */

#include <algorithm>
#include <vector>
#include <string>

/*
 * This is basically our library-free code to parse our arguments, it is obviously not feature full for the same purporse
 * we created this. Basically, we use it so that we don't have to compile a 1MiB header file to just parse our options.
 * Please, future self or future maintainers, do only replace this if we have a minimal library that can actually do this better.
 *
 * TODO: Find a smart way to parse multiple bool inline commands such as '-abcdef'.
 * TODO: Only use and remove the first match.
 */
class InputParser
{
  private:
    std::vector <std::string> tokens;

  public:

    InputParser (const int argc, char ** argv )
    {
      // Loop through each argument and add it to the tokens private variable.
      for ( int i = 1; i < argc; ++i )
      {
        this->tokens.push_back( std::string(argv[i]) );
      }
    }

    /* This will return the value after the option, so it verifies two strings.
     * It doesn't matter how you call it, but for both strings it will verify and take the next argument.
     * It will actually remove from the leftovers both the strings even if it just took the first one.
     * Original author is @iain, though it is modified for our needs.
     * 
    */
    const std::string
    get( const std::string & short_name, const std::string & long_name )
    {
      std::vector <std::string>::const_iterator itr;

      #define GET_STRING(A)		\
          itr = find( this -> tokens.begin(), this -> tokens.end(), A );		\
          remove(this->tokens.begin(), this->tokens.end(), A);		\
          if ( itr != this->tokens.end() && ++itr != this->tokens.end() )

      GET_STRING(short_name) {
        const std::string parameter = *--itr;
        remove(this->tokens.begin(), this->tokens.end(), parameter);
        remove(this->tokens.begin(), this->tokens.end(), long_name);
        return parameter;
      }

      GET_STRING(long_name) {
        const std::string parameter = *--itr;
        remove(this->tokens.begin(), this->tokens.end(), parameter);
        return parameter;
      }

      // Return nothing if it doesn't find.
      static const std::string empty_string("");
      return empty_string;
    }

    /*
     * This will return the rest of our arguments after all the calls are made, such as this->get or this->exist.
     * Use this will great knowledge about what you're doing.
     */
    const std::string
    left ( void ) const {

      std::string s;

      // Add all the arguments from the vector into a string.
      for (std::string word : tokens)
      {
        s += " " + word;
      }

      // Find trailing characters from the left side and remove them.
      size_t start = s.find_first_not_of(" \n\r\t\f\v");
      s =  (start == std::string::npos) ? "" : s.substr(start);

      // Find trailing characters from the right side and remove them.
      size_t end = s.find_last_not_of(" \n\r\t\f\v");
      s = (end == std::string::npos) ? "" : s.substr(0, end + 1);

      return s;
    }

    /* To return true if an option was passed to the arguments. It will remove from this->tokens the strings it finds.
     * short_name : -a
     * long_name  : --animal
    */
    bool
    exists( const std::string & short_name, const std::string & long_name )
    {
     
      #define EXIST_STRING(A) ( find( this->tokens.begin(), this->tokens.end(), A ) != this->tokens.end() )

      // Loop through each option to see if it exists there.
      if ( EXIST_STRING(short_name) || EXIST_STRING(long_name) ) {

        remove(this->tokens.begin(), this->tokens.end(), short_name);
        remove(this->tokens.begin(), this->tokens.end(), long_name);

        return true;

      };

      return false;

    }

}
