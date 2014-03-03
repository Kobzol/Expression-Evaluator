#include "Evaluator.h"

namespace eval
{
	EvaluationResult::EvaluationResult()
	{
		this->success = false;
		this->result = 0.0;
	}

	bool EvaluationResult::isSuccessful()
	{
		return this->success;
	}
	double EvaluationResult::getResult()
	{
		return this->result;
	}
	void EvaluationResult::setResult(double result)
	{
		this->result = result;
		this->success = true;
	}

	bool Evaluator::isDigit(char token)
	{
		return isdigit((int)token) != 0;
	}
	bool Evaluator::isOperator(char token)
	{
		if (precedences.find(token) != precedences.end())
		{
			return true;
		}

		return false;
	}
	bool Evaluator::isWhiteSpace(char token)
	{
		return isspace((int)token) != 0;
	}
	bool Evaluator::isFloatingPoint(char token)
	{
		if (token == '.' || token == ',')
		{
			return true;
		}
		else return false;
	}
	bool Evaluator::isHyphen(char token)
	{
		if (token == '-')
		{
			return true;
		}
		else return false;
	}
	bool Evaluator::isLeftParentheses(char token)
	{
		if (token == '(')
		{
			return true;
		}
		else return false;
	}
	bool Evaluator::isRightParentheses(char token)
	{
		if (token == ')')
		{
			return true;
		}
		else return false;
	}
	bool Evaluator::isNumberToken(char token)
	{
		if (Evaluator::isDigit(token) || Evaluator::isFloatingPoint(token))
		{
			return true;
		}
		else return false;
	}
	unsigned int Evaluator::getOperatorPrecedence(char operatorSymbol)
	{
		if (Evaluator::isOperator(operatorSymbol))
		{
			return precedences[operatorSymbol];
		}
		else return 0;
	}

	EvaluationResult Evaluator::stringToNumber(std::string input)
	{
		EvaluationResult result;
		double number;

		bool foundPoint = false;

		for (size_t i = 0; i < input.size(); i++)
		{
			char token = input[i];

			if (Evaluator::isFloatingPoint(token))
			{
				if (foundPoint)
				{
					return result;
				}

				foundPoint = true;
			}
			else if (Evaluator::isHyphen(token))
			{
				if (i != 0)
				{
					return result;
				}
			}
			else if (Evaluator::isDigit(token) == false)
			{
				return result;
			}
		}

		if (input.size() > 0)
		{
			std::stringstream(input) >> number;
			result.setResult(number);
		}

		return result;
	}
	std::string Evaluator::convertToInfix(std::string expr)
	{
		std::stack<char> symbols;

		std::string infix;
		std::string operand;
		std::string converted;

		for (size_t i = 0; i < expr.size(); i++)
		{
			char token = expr[i];

			if (Evaluator::isNumberToken(token))
			{
				while (true)
				{
					operand.push_back(token);

					if (i < expr.size() - 1 && (Evaluator::isDigit(expr[i + 1]) || Evaluator::isFloatingPoint(expr[i + 1])))
					{
						token = expr[++i];
					}
					else break;
				}

				EvaluationResult result;

				result = stringToNumber(operand);

				if (result.isSuccessful())
				{
					std::stringstream converter;

					converter << result.getResult();

					infix.append(converter.str() + " ");
				}

				operand.clear();
			}
			else if (Evaluator::isLeftParentheses(token))
			{
				symbols.push(token);
			}
			else if (Evaluator::isRightParentheses(token))
			{
				while (symbols.empty() == false && symbols.top() != '(')
				{
					converted = symbols.top();
					infix.append(converted + " ");
					symbols.pop();
				}

				if (symbols.empty() == false && symbols.top() == '(')
				{
					symbols.pop();
				}
			}
			else if (Evaluator::isOperator(token))
			{
				if (Evaluator::isHyphen(token))
				{
					if (i != 0 && (Evaluator::isOperator(expr[i - 1]) || Evaluator::isLeftParentheses(expr[i - 1])))
					{
						operand.push_back(token);
						continue;
					}
				}

				char operatorSymbol;

				while (symbols.empty() == false && Evaluator::isOperator(operatorSymbol = symbols.top()))
				{
					if (Evaluator::getOperatorPrecedence(token) <= Evaluator::getOperatorPrecedence(operatorSymbol))
					{
						converted = operatorSymbol;
						infix.append(converted + " ");

						symbols.pop();
					}
					else break;
				}

				symbols.push(token);
			}
		}

		while (symbols.empty() == false)
		{
			converted = symbols.top();
			infix.append(converted + " ");
			symbols.pop();
		}

		return infix;
	}
	bool Evaluator::testParentheses(std::string expr)
	{
		int parenthesesCount = 0;

		for (size_t i = 0; i < expr.size(); i++)
		{
			char token = expr[i];

			if (Evaluator::isLeftParentheses(token))
			{
				parenthesesCount++;
			}
			else if (Evaluator::isRightParentheses(token))
			{
				if (parenthesesCount < 1)
				{
					return false;
				}
				else parenthesesCount--;
			}
		}

		return parenthesesCount == 0;
	}

	EvaluationResult Evaluator::computeOperation(char operatorToken, std::stack<double> &tokens)
	{
		EvaluationResult result;

		if (tokens.size() > 1)
		{
			double a = tokens.top();
			tokens.pop();

			double b = tokens.top();
			tokens.pop();

			switch (operatorToken)
			{
			case '+':
				result.setResult(b + a);
				break;
			case '-':
				result.setResult(b - a);
				break;
			case '/':
			{
				if (a != 0)
				{
					result.setResult(b / a);
				}

				break;
			}
			case '*':
				result.setResult(b * a);
				break;
			case '%':
				result.setResult(fmod(b, a));
				break;
			case '^':
				result.setResult(pow(b, a));
				break;
			}
		}

		return result;
	}
	EvaluationResult Evaluator::EvaluateInfix(std::string expr)
	{
		EvaluationResult result;
		std::stack<double> tokens;

		bool success = true;

		std::string operand;

		if (Evaluator::testParentheses(expr) == false)
		{
			return result;
		}

		expr = "(" + expr + ")";
		expr = Evaluator::convertToInfix(expr);

		for (size_t i = 0; i < expr.size(); i++)
		{
			char token = expr[i];

			if (Evaluator::isNumberToken(token))
			{
				operand.push_back(token);
			}
			else if (Evaluator::isOperator(token))
			{
				if (Evaluator::isHyphen(token))
				{
					if (i != expr.size() - 1 && Evaluator::isWhiteSpace(expr[i + 1]) == false)
					{
						operand.push_back(token);
						continue;
					}
				}

				EvaluationResult computation = computeOperation(token, tokens);

				if (computation.isSuccessful())
				{
					tokens.push(computation.getResult());
				}
				else
				{
					success = false;
					break;
				}
			}
			else if (Evaluator::isWhiteSpace(token))
			{
				EvaluationResult result;

				result = stringToNumber(operand);

				if (result.isSuccessful())
				{
					tokens.push(result.getResult());
					operand.clear();
				}
			}
			else
			{
				success = false;
				break;
			}
		}

		if (success && tokens.size() == 1)
		{
			double resultData = tokens.top();
			result.setResult(resultData);
		}

		return result;
	}

	std::map<char, unsigned int> Evaluator::precedences =
	{
		{ '+', 1 }
		, { '-', 1 }
		, { '*', 2 }
		, { '/', 2 }
		, { '%', 2 }
		, { '^', 3 }
	};

}