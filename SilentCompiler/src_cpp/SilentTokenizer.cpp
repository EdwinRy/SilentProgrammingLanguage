#include "SilentTokenizer.hpp"
namespace SilentTokenizer
{
    std::vector<silentToken>* silentTokenize(std::string source)
    {
        std::vector<silentToken>* tokens = new std::vector<silentToken>();
        unsigned int currentLine = 1;
        for(unsigned int i = 0; i < source.length(); i++)
        {
            if(source[i] == '\n')
            {
                currentLine += 1;
                continue;
            }

            if(source[i] == '/' && source[i+1] == '/')
            {
                while(source[i] != '\n')
                {
                    i++;
                }
                i--;
                continue;
            }

            if(source[i] == '/' && source[i+1] == '*')
            {
                while(!(source[i] == '*' && source[i+1] == '/' ))
                {
                    if(source[i] == '\n')
                    {
                        currentLine+=1;
                    }
                    i++;
                }
                i++;
                continue;
            }

            silentToken token;
            token.type = silentUnrecognisedToken;
            token.value = "";
            token.currentLine = currentLine;

            if(source[i] == ';')
            {
                token.type = silentOtherToken;
                token.value = ";";
            }

            else if(source[i] == '.')
            {
                token.type = silentOtherToken;
                token.value = ".";
            }

            else if(source[i] == ',')
            {
                token.type = silentOtherToken;
                token.value = ",";
            }

            else if(source[i] == '=')
            {
                token.type = silentExpressionToken;
                token.value = "=";
            }

            else if(source[i] == '+')
            {
                token.type = silentMathsOperatorToken;
                token.value = "+";
            }

            else if(source[i] == '-')
            {
                token.type = silentMathsOperatorToken;
                token.value = "-";
            }

            else if(source[i] == '/')
            {
                token.type = silentMathsOperatorToken;
                token.value = "/";
            }

            else if(source[i] == '*')
            {
                token.type = silentMathsOperatorToken;
                token.value = "*";
            }

            else if(source[i] == '(')
            {
                token.type = silentOpenBracketToken;
                token.value = "(";
            }

            else if(source[i] == ')')
            {
                token.type = silentClosingBracketToken;
                token.value = ")";
            }

            else if(source[i] == '{')
            {
                token.type = silentOpenBracketToken;
                token.value = "{";
            }

            else if(source[i] == '}')
            {
                token.type = silentClosingBracketToken;
                token.value = "}";
            }

            else if(source[i] == '\"')
            {
                token.value = silentStringToken;
                i++;
                while(source[i] != '\"')
                {
                    if(source[i] == '\\')
                    {
                        i += 1;
                        if(source[i] == 'n')
                        {
                            token.value += '\n';
                        }
                        else if(source[i] == 't')
                        {
                            token.value += '\t';
                        }
                        else if(source[i] == '\"')
                        {
                            token.value += '\"';
                        }
                        else if(source[i] == '\\')
                        {
                            token.value += '\\';
                        }
                        else if(source[i] == 'v')
                        {
                            token.value += '\v';
                        }
                        else if(source[i] == 'r')
                        {
                            token.value += '\r';
                        }
                        else if(source[i] == 'f')
                        {
                            token.value += '\f';
                        }
                        else if(source[i] == '0')
                        {
                            token.value += '\0';
                        }
                        else
                        {
                            printf("Incorrect string formatting on line %i\n",
                                currentLine);
                        }
                        i++;
                        continue;
                    }
                    token.value += source[i];
                    i++;
                }
            }

            else if(isalpha(source[i]))
            {
                while(isalpha(source[i]) || isdigit(source[i]))
                {
                    token.value += source[i];
                    i++;
                }
                i--;
                if(token.value == "func")
                {
                    token.type = silentStructureToken;
                }
                else if(token.value == "struct")
                {
                    token.type = silentStructureToken;
                }
                else if(token.value == "var")
                {
                    token.type = silentStructureToken;
                }
                else if(token.value == "int")
                {
                    token.type = silentTypeToken;
                }
                else if(token.value == "float")
                {
                    token.type = silentTypeToken;
                }
                else if(token.value == "long")
                {
                    token.type = silentTypeToken;
                }
                else if(token.value == "double")
                {
                    token.type = silentTypeToken;
                }
                else if(token.value == "string")
                {
                    token.type = silentTypeToken;
                }
                else
                {
                    token.type = silentIdentifierToken;
                }
            }

            else if(isdigit(source[i]))
            {
                while(isdigit(source[i]) || source[i] == '.')
                {
                    token.value += source[i];
                    i++;
                }
                i--;
                token.type = silentNumberToken;
            }
            else
            {
                continue;
            }
            tokens->push_back(token);
            //std::cout << token.value << std::endl; 
        }
        return tokens;
    }
}