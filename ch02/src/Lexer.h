//
// Created by 84648 on 2024/11/13.
//

#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h" // 提供对内存块的只读访问

class Lexer;

class Token {
    friend class Lexer;

public:
    // 唯一标记符号的枚举定义
    enum TokenKind : unsigned short {
        // eoi 表示输入结束，当处理完输入的所有字符时返回
        // unknown 表示词法级别错误时的映射
        eoi, unknown, ident, number, comma, colon, plus,
        minus, star, slash, l_paren, r_paren, KW_with
    };

private:
    Tokenkind Kind;
    llvm::StringRef Text; // 指向标记文本的开头（很有用的设计）

public:
    TokenKind getKind() const {
        return Kind;
    }
    llvm::StringRef getText() const {
        return Text;
    }

    bool is(TokenKind K) const {
        return Kind == K;
    }
    bool isOneOf(TokenKind K1, TokenKind K2) const {
        return is(K1) || is(K2);
    }
    template <typename... Ts> // 可变模板，允许参数数量可变
    bool isOneOf(TokenKind K1, TokenKind K2, Ts... Ks) const {
        return is(K1) || isOneof(K2, Ks...)
    }
};

class Lexer {
    const char *BufferStart;
    const char *BufferPtr;

public:
    Lexer(const llvm::StringRef &Buffer) {
        BufferStart = Buffer.begin();
        BufferPtr = BufferStart;
    }

    void next(Token &token); // 用于返回下一个标识，类似于迭代器

private:
    void formToken(Token &Result, const char *TokEnd, Token::TokenKind Kind);
};

#endif //LEXER_H
