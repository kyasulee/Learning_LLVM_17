//
// Created by 84648 on 2024/11/13.
//
#include "Lexer.h"

namespace charinfo {
    // 空格
    LLVM_READNONE inline bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\f' || c == '\v' || c == '\r' || c == '\n';
    }
    // 数字
    LLVM_READNONE inline bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    // 字母
    LLVM_READNONE inline bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
}

void Lexer::next(Token &token) {
    // 跳过空格
    while(*BufferPtr && charinfo::isWhitespace(*BufferPtr)) {
        ++BufferPtr;
    }
    // 是否到结尾
    if(!*BufferPtr) {
        token.Kind = Token::eoi;
        return;
    }
    if (charinfo::isLetter(*BufferPtr)) {
        // 找当前单词的结尾位置
        const char *end = BufferPtr + 1;
        while (charinfo::isLetter(*end))
            ++end;

        // 指向单词开头的指针和这个单词的长度
        llvm::StringRef Name(BufferPtr, end - BufferPtr)
        // 判断是否为关键字with
        Token::TokenKind Kind = Name == "with" ? Token::KW_with : Token::ident;
        // 填充标记
        formToken(token, end, Kind);
        return;
    } else if (charinfo::isDigit(*BufferPtr)) {
        const char *end = BufferPtr + 1;
        while (charinfo::isDigit(*end))
            ++end;
        formToken(token, end, Token::number);
        return;
    } else {
        switch (*BufferPtr) {
            // 使用预处理宏来减少代码量
            #define CASE(ch, tok) case ch: formToken(token, BufferPtr + 1, tok); break
                CASE('+', Token::plus);
                CASE('-', Token::minus);
                CASE('*', Token::star);
                CASE('/', Token::slash);
                CASE('(', Token::Token::l_paren);
                CASE(')', Token::Token::r_paren);
                CASE(':', Token::Token::colon);
                CASE(',', Token::Token::comma);
            #undef CASE
            default:
                formToken(token, BufferPtr + 1, Token::unknown);
        }
        return;
    }
}

// 填充Token实例成员，并更新指向下一个未处理字符的指针
void Lexer::formToken(Token &Tok,
                      const char *TokEnd,
                      Token::TokenKind Kind) {
    Tok.Kind = Kind;
    Tok.Text = llvm::StringRef(BufferPtr, TokEnd - BufferPtr);
    BufferPtr = TokEnd;
}