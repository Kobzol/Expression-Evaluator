#pragma once

#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <sstream>
#include <map>

namespace eval
{
	struct EvaluationResult
	{
	private:
		bool success;
		double result;
	
	public:
		EvaluationResult()
		{
			this->success = false;
			this->result = 0.0;
		}
		
		bool isSuccessful();
		double getResult();
		void setResult(double result);
	};
	
	class Evaluator
	{
	private:
		static std::map<char, unsigned int> precedences;

		static bool isOperator(char token);
		static bool isDigit(char token);
		static bool isWhiteSpace(char token);
		static bool isFloatingPoint(char token);
		static bool isHyphen(char token);
		static bool isLeftParentheses(char token);
		static bool isRightParentheses(char token);
		static bool isNumberToken(char token);
		static unsigned int getOperatorPrecedence(char operatorSymbol);

		static EvaluationResult Evaluator::stringToNumber(std::string input);
		static std::string convertToInfix(std::string expr);
		static bool testParentheses(std::string expr);

		static EvaluationResult Evaluator::computeOperation(char operatorToken, std::stack<double> &tokens);
	public:
		static EvaluationResult EvaluateInfix(std::string expr);		
	};
}