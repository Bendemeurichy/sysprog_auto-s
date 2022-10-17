#ifndef SYSPROG_ASSEMBLER_H
#define SYSPROG_ASSEMBLER_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <cstdint>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <regex>

#include "Constants.h"
#include "CPUInstruction.h"

/**
 * AT&T style assembler:
 *
 * .equ CONST_Y, 8
 * LABEL_U:
 *    .long 8
 *    .string "blah"
 * .LABEL_V:
 *    OP %REG, $IMMEDIATE_NR
 *    OP -8(%REG), 12($IMMEDIATE_NR)
 *    OP $CONST_X($LABEL_A), 32($LABEL_B)
 *    OP $CONST_X(LABEL_A), 32(LABEL_B)
 * //you can use labels with or without $  (?)
 *
 * .equ IO_ACTION_START, 0x73
 * .equ BOT_IDLE, 0
 * .equ DECISION_FORWARD, 1
 * .equ DECISION_BACK, 2
 *    movl %sp, %bp
 *    movq $IO_ACTION_START, %a
 *    movb $BOT_IDLE, (%a)
 */

class AssemblerError : public std::runtime_error {
public:
    explicit AssemblerError(const std::string & msg);
    explicit AssemblerError(const char * msg);
};

class AssemblerErrorWithLine : public std::runtime_error {
private:
    const int line_nr;
    const std::string line_content;
public:
    AssemblerErrorWithLine(const AssemblerErrorWithLine&) noexcept;
    AssemblerErrorWithLine(int line_nr, const std::string & msg, std::string line_content);
    AssemblerErrorWithLine(int line_nr, const char * msg, std::string line_content);
};

typedef enum TType {
    EMPTY,      //empty space...
    LABEL,      //LabelName:
    DATASPEC,   // .(byte|word) data
    COMMENT,    //   //  /*   #
    DATA_NR,    //  [0-9]* or 0x[0-9]*
    OPERATION,
    OPERAND,
    EQU,        //.equ
    EQU_ID,     //id after equ
    PARSE_ERROR_DOTUNKNOWN,
    PARSE_ERROR_OTHER
} TType;

class AssemblerToken {
public:
    const TType tokenType;
    const std::string val;     //value, stripped of syntax "decorators", like : . " 0x and ,
    AssemblerToken(const TType& tokenType, const std::string &val);
};


class Assembler {
#ifdef ONLY_IN_PART2_TESTS
//for easy testing, we allow messing with assembler internals
public:
#else
private:
#endif
    const std::regex label_regex;
    const std::regex dataspec_regex;
    const std::regex comment_regex;
    const std::regex data_nr_regex;
    const std::regex operation_regex;
    const std::regex operand_regex;
    const std::regex equ_regex;
    const std::regex equid_regex;
    
    const std::regex operand_reg_regex;
    const std::regex operand_imm_regex;
    const std::regex operand_address_regex;
    const std::regex operand_mem_reg_regex;
    
    const spg_addr_t start_addr_code;

    const std::unordered_map<std::string, CPUInstruction::Operation> op_map;

    uint8_t* target;
    uint8_t* cur_target;
    size_t target_size;

    std::map<std::string, spg_register_t> constants; //could be filled in single pass          (no forward references) (allows re-assign)
    std::map<std::string, spg_addr_t> labels;        //must be filled in on a first pass of 2  (forward references)
public:
    explicit Assembler(spg_addr_t start_addr_code);

    /**
     *
     * @param source_lines
     * @param target
     * @param target_size
     * @return the number of bytes written to the target
     */
    size_t assemble(const std::vector<std::string> &source_lines, uint8_t *target, size_t target_size);
    
    
    std::vector<AssemblerToken> tokenize(const std::string& line);
    
    bool has_parse_error(const std::vector<AssemblerToken>& tokens);
    bool is_data(const std::vector<AssemblerToken>& tokens);         //.(byte|word|long||quad|string|addr) data
    bool is_const_declare(const std::vector<AssemblerToken>& tokens);        //.equ const_name, val
    bool is_label(const std::vector<AssemblerToken>& tokens);        //label_name:
    bool is_instruction(const std::vector<AssemblerToken>& tokens);
    
    int to_data_bytecount(const AssemblerToken& data_token);
    size_t get_data_byte_size(const std::vector<AssemblerToken>& tokens);
    size_t get_instruction_bit_size(const std::vector<AssemblerToken>& tokens);
    uint8_t get_instruction_byte_size(const std::vector<AssemblerToken>& tokens);
    uint8_t get_instruction_byte_size(const CPUInstruction::Instruction& instruction);
    
    spg_register_t to_nr(const AssemblerToken& token);
    CPUInstruction::Operation to_operation_internal(const std::string &op_str);
    CPUInstruction::Operation to_operation(const AssemblerToken &token);
    uint8_t reg_name_to_index(const std::string& reg_name);
    size_t to_operand_bitsize(const AssemblerToken& token);
    std::shared_ptr<CPUInstruction::Operand> to_operand(const AssemblerToken& token);
    CPUInstruction::Instruction to_instruction(const std::vector<AssemblerToken>& tokens);
    
    void write_nr(spg_register_t nr, int byte_count);
    void write_data(const std::vector<AssemblerToken>& tokens);
    void write_instruction(const CPUInstruction::Instruction& instruction); //write instruction bytes to cur_target
};

std::vector<std::string> split(const std::string& in, const std::string& delim);

std::vector<uint8_t> assemble(const std::vector<std::string> &asmLinesIn, spg_addr_t start_addr_code);

#endif //SYSPROG_ASSEMBLER_H
