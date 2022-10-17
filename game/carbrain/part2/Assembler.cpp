#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#include "Assembler.h"
#include "../carbrain_io.h"

#include <assert.h>
#include <fstream>
#include <utility>
#include <vector>
#include <stdint.h>

using namespace CPUInstruction;

std::vector<std::string> split(const std::string& in, const std::string& delim) {
    std::vector<std::string> res;
    size_t start = 0, end = 0;
    
    while (end != std::string::npos) {
        end = in.find(delim, start);
        std::string part = in.substr(start, end == std::string::npos ? std::string::npos : end - start);
        if (!part.empty()) res.push_back(part);
        start = end > (std::string::npos - delim.size()) ? std::string::npos : end + delim.size();
    }
    
    return res;
}

AssemblerError::AssemblerError(const std::string &msg) : runtime_error(msg) {  }
AssemblerError::AssemblerError(const char *msg) : runtime_error(msg) {  }

AssemblerErrorWithLine::AssemblerErrorWithLine(int line_nr, const std::string &msg, std::string line_content) :
        runtime_error(msg + std::string(" @ line ") + std::to_string(line_nr) + std::string(": ") + line_content),
        line_nr(line_nr),
        line_content(std::move(line_content))
{ }
AssemblerErrorWithLine::AssemblerErrorWithLine(int line_nr, const char *msg, std::string line_content) :
        runtime_error(std::string(msg) + std::string(" @ line ") + std::to_string(line_nr) + std::string(": ") + line_content),
        line_nr(line_nr),
        line_content(std::move(line_content))
{  }

AssemblerErrorWithLine::AssemblerErrorWithLine(const AssemblerErrorWithLine & orig) noexcept  :
    runtime_error(orig.what()),
    line_nr(orig.line_nr),
    line_content(orig.line_content) {
}

static_assert(std::is_nothrow_copy_constructible<AssemblerError>::value,
                   "AssemblerError must be nothrow copy constructible");
static_assert(std::is_nothrow_copy_constructible<AssemblerErrorWithLine>::value,
                   "AssemblerErrorWithLine must be nothrow copy constructible");


AssemblerToken::AssemblerToken(const TType& tokenType, const std::string &val) : tokenType(tokenType), val(val) {}

static std::unordered_map<std::string, CPUInstruction::Operation> init_op_map() {
    std::unordered_map<std::string, CPUInstruction::Operation> op_map;
    op_map["NOP"] = NOP;
    op_map["MOV"] = MOV;
    op_map["MOVB"] = MOVB;
    op_map["PUSH"] = PUSH;
    op_map["POP"] = POP;
    op_map["ADD"] = ADD;
    op_map["SUB"] = SUB;
    op_map["AND"] = AND;
    op_map["OR"] = OR;
    op_map["XOR"] = XOR;
    op_map["NOT"] = NOT;
    op_map["SHR"] = SHR;
    op_map["SHL"] = SHL;
    op_map["INC"] = INC;
    op_map["DEC"] = DEC;
    op_map["CMP"] = CMP;
    op_map["TEST"] = TEST;
    op_map["JMP"] = JMP;
    op_map["JE"] = JE;
    op_map["JZ"] = JE; //JZ and JE do exactly the same
    op_map["JNE"] = JNE;
    op_map["JNZ"] = JNE; //JNZ and JNE do exactly the same
    op_map["CALL"] = CALL;
    op_map["RET"] = RET;
    return op_map;
}

Assembler::Assembler(spg_addr_t start_addr_code)
        : label_regex("^([A-Za-z_][A-Za-z_0-9-]*):$", std::regex_constants::ECMAScript),
          dataspec_regex("^\\.(byte|word)$", std::regex_constants::ECMAScript),
          comment_regex("^(//|#).*", std::regex_constants::ECMAScript),
          data_nr_regex("^([+-]?[0-9]+|0x[0-9a-fA-F]+),?$", std::regex_constants::ECMAScript),
          operation_regex("^([A-Za-z]+)$", std::regex_constants::ECMAScript),
          operand_regex("^([A-Za-z%$)_(0-9-]+),?$", std::regex_constants::ECMAScript),
          equ_regex("^\\.(equ)$", std::regex_constants::ECMAScript),
          equid_regex("^([a-zA-Z][a-zA-Z0-9_]*)$", std::regex_constants::ECMAScript),
          operand_reg_regex("^%([a-z]+|r[0-9]+)$", std::regex_constants::ECMAScript),
          operand_imm_regex("^\\$([A-Za-z_0-9-]+)$", std::regex_constants::ECMAScript),
          operand_address_regex("^\\(\\$([A-Za-z_0-9-]+)\\)$", std::regex_constants::ECMAScript),
          operand_mem_reg_regex("^([-+]?[0-9]+)?\\(%([a-z]+|r[0-9]+)\\)$", std::regex_constants::ECMAScript),
          start_addr_code(start_addr_code),
          op_map(init_op_map()) {
}


size_t Assembler::assemble(const std::vector<std::string> &source_lines, uint8_t *target, size_t target_size) {
    this->target = target;
    this->cur_target = target;
    this->target_size = target_size;
    
    //1st pass
    int line_nr = 0;
    spg_addr_t cur_instr_addr = start_addr_code;
    for (std::vector<std::string>::const_iterator it = source_lines.begin(); it != source_lines.end(); it++) {
        const std::string &line = *it;
        try {
            const std::vector<AssemblerToken> tokens = tokenize(line);
            bool handled = false;
            if (has_parse_error(tokens)) {
                throw AssemblerErrorWithLine(line_nr, "line failed to parse", line);
            }
            if (is_label(tokens)) {
                assert(!handled);
                assert(tokens.size() == 1);
                labels[tokens[0].val] = cur_instr_addr;
                handled = true;
            }
            if (is_const_declare(tokens)) {
                assert(!handled);
                assert(tokens.size() == 3);
                assert(tokens[1].tokenType == EQU_ID);
                assert(tokens[2].tokenType == DATA_NR);
                constants[tokens[1].val] = to_nr(tokens[2]);
                handled = true;
            }
            if (is_instruction(tokens)) {
                assert(!handled);
                //std::cout << "   " << (cur_instr_addr - start_addr_code) << ": " << line << std::endl;
                cur_instr_addr += get_instruction_byte_size(tokens);
                handled = true;
            }
            if (is_data(tokens)) {
                assert(!handled);
                cur_instr_addr += get_data_byte_size(tokens);
                handled = true;
            }
            line_nr++;
        } catch (const AssemblerError& e) {
            std::cerr << "Failed to assemble: " << e.what() << std::endl;
            throw AssemblerErrorWithLine(line_nr, e.what(), line);
        }
    }
    
    //2nd pass
    line_nr = 0;
    cur_instr_addr = start_addr_code;
    for (std::vector<std::string>::const_iterator it = source_lines.begin(); it != source_lines.end(); it++) {
        const std::string &line = *it;
        try {
            const std::vector<AssemblerToken> tokens = tokenize(line);
            if (is_instruction(tokens)) {
                Instruction instruction = to_instruction(tokens);
                write_instruction(instruction);
                int inst_byte_size = get_instruction_byte_size(tokens);
                cur_instr_addr += inst_byte_size;
                int byte_m1 = cur_target - target;
                int byte_m2 = cur_instr_addr - start_addr_code;
                assert(byte_m1 == byte_m2);
            }
            if (is_data(tokens)) {
                cur_instr_addr += get_data_byte_size(tokens);
                write_data(tokens);
            }
            line_nr++;
        } catch (const AssemblerError& e) {
            std::cerr << "Failed to assemble: " << e.what() << std::endl;
            throw AssemblerErrorWithLine(line_nr, e.what(), line);
        }
    }
    
    return this->cur_target - this->target;
}




std::vector<AssemblerToken> Assembler::tokenize(const std::string &line) {
    std::vector<std::string> words = split(line, " ");
    std::vector<AssemblerToken> res;
    TType prev_tokenType = EMPTY;
    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++) {
        const std::string& word = *it;
        if (word.empty()) continue;
    
        std::smatch sm;
        //first of all, whatever was previously seen, search, search for a new comment start
        if (regex_search(word, sm, comment_regex)) {
            prev_tokenType = COMMENT;
            continue;
        }
        
        switch (prev_tokenType) {
            case PARSE_ERROR_OTHER:
            case LABEL: {
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
            case COMMENT: continue;
            case EMPTY: {
                if (regex_search(word, sm, comment_regex)) {
                    prev_tokenType = COMMENT;
                    continue;
                }
                if (regex_search(word, sm, label_regex)) {
                    prev_tokenType = LABEL;
                    res.emplace_back(LABEL, sm[1]);
                    continue;
                }
                if (regex_search(word, sm, dataspec_regex)) {
                    res.emplace_back(DATASPEC, sm[1]);
                    prev_tokenType = DATASPEC;
                    continue;
                }
                if (regex_search(word, sm, equ_regex)) {
                    res.emplace_back(EQU, sm[1]);
                    prev_tokenType = EQU;
                    continue;
                }
                if (regex_search(word, sm, operation_regex)) {
                    try {
                        to_operation_internal(word);
                        res.emplace_back(OPERATION, sm[1]);
                        prev_tokenType = OPERATION;
                        continue;
                    } catch (AssemblerError& err) {
                        //This was not a valid operation
                        //Do nothing -> PARSE_ERROR_OTHER below
                    }
                }
                if (!word.empty() && word[0] == '.') {
                    prev_tokenType = PARSE_ERROR_DOTUNKNOWN;
                    res.emplace_back(PARSE_ERROR_DOTUNKNOWN, word);
                    continue;
                }
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
            case DATASPEC: {
                if (regex_search(word, sm, data_nr_regex)) {
                    prev_tokenType = DATASPEC;
                    res.emplace_back(DATA_NR, sm[1]);
                    continue;
                }
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
            case OPERAND:
            case OPERATION: {
                if (regex_search(word, sm, operand_regex)) {
                    prev_tokenType = OPERAND;
                    res.emplace_back(OPERAND, sm[1]);
                    continue;
                }
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
                
            }
            case EQU: {
                if (regex_search(word, sm, equid_regex)) {
                    prev_tokenType = EQU_ID;
                    res.emplace_back(EQU_ID, sm[1]);
                    continue;
                }
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
            case EQU_ID: {
                if (regex_search(word, sm, data_nr_regex)) {
                    prev_tokenType = DATA_NR;
                    res.emplace_back(DATA_NR, sm[1]);
                    continue;
                }
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
            default: {
                prev_tokenType = PARSE_ERROR_OTHER;
                res.emplace_back(PARSE_ERROR_OTHER, word);
                continue;
            }
        }
    }
    return res;
}


bool Assembler::has_parse_error(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return false;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].tokenType == PARSE_ERROR_OTHER)
            return true;
        if (tokens[i].tokenType == PARSE_ERROR_DOTUNKNOWN)
            return true;
    }
    return false;
}


bool Assembler::is_data(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return false;
    return tokens[0].tokenType == DATASPEC;
}


bool Assembler::is_const_declare(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return false;
    return tokens[0].tokenType == EQU;
}


bool Assembler::is_label(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return false;
    return tokens[0].tokenType == LABEL;
}


bool Assembler::is_instruction(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return false;
    return tokens[0].tokenType == OPERATION;
}


int Assembler::to_data_bytecount(const AssemblerToken &data_token) {
    assert(data_token.tokenType == DATASPEC);
    if (data_token.val == "byte") return 1;
    if (data_token.val == "word") return 2;
    return -1;
}


size_t Assembler::get_data_byte_size(const std::vector<AssemblerToken>& tokens) {
    if (tokens.empty()) return 0;
    int data_bytecount = to_data_bytecount(tokens[0]);
    if (data_bytecount > 0) return data_bytecount * (tokens.size() - 1);
    if (tokens[0].val == "string") {
        size_t res = 0;
        //TODO: take \0 \n etc into account  (or decide we don't do that)
        for (size_t i = 1; i < tokens.size(); i++) {
            res += tokens[i].val.size();
        }
        return res;
    }
    throw AssemblerError(std::string("Error parsing '") + tokens[0].val + std::string("' to data"));
}


uint8_t Assembler::get_instruction_byte_size(const std::vector<AssemblerToken>& tokens) {
    return (uint8_t) (((get_instruction_bit_size(tokens)-1) / 8)+1);
}
size_t Assembler::get_instruction_bit_size(const std::vector<AssemblerToken>& tokens) {
    assert(!tokens.empty());
    Operation op = to_operation(tokens[0]);
    size_t bitSize = 5;
    
    int operand_count = Instruction::operationToOperandCount(op);
    if (operand_count < 0)
        throw AssemblerError(std::string("instruction '") + tokens[0].val +
                             std::string("' has bad operand count ") + std::to_string(operand_count));
    if (operand_count == 0)
        return bitSize;
    assert (operand_count >= 1);

    if (tokens.size() < 2)
        throw AssemblerError(std::string("instruction is missing 1st operand) '" + tokens[0].val + std::string("'")));

    bitSize += to_operand_bitsize(tokens[1]);
    if (operand_count == 1)
        return bitSize;
    assert (operand_count == 2);

    if (tokens.size() < 3)
        throw AssemblerError(std::string("instruction is missing 2nd operand '" + tokens[0].val + std::string("'")));

    bitSize += to_operand_bitsize(tokens[2]);
    return bitSize;
}


uint8_t Assembler::get_instruction_byte_size(const Instruction &instruction) {
    uint8_t res = (uint8_t) instruction.encode([](uint8_t byte){});
    return res;
}


spg_register_t Assembler::to_nr(const AssemblerToken &token) {
    assert(token.tokenType == DATA_NR);
    //see https://en.cppreference.com/w/cpp/string/basic_string/stoul
    return std::stoul(token.val, 0, 0); //automatically handles hex as well!
}


Instruction Assembler::to_instruction(const std::vector<AssemblerToken>& tokens) {
    assert(!tokens.empty());
    Operation op = to_operation(tokens[0]);
    
    int operand_count = Instruction::operationToOperandCount(op);
    if (operand_count == 0) {
        return Instruction(op);
    }
    if (operand_count >= 1) {
        if (tokens.size() < 2) {
            throw AssemblerError(std::string("instruction is missing 1st operand) '" + tokens[0].val + std::string("'")));
        }
        
        std::shared_ptr<Operand> source = to_operand(tokens[1]);
        if (operand_count == 1) {
            return Instruction(op, source);
        }
        
        if (tokens.size() < 3) {
            throw AssemblerError(std::string("instruction is missing 2nd operand '" + tokens[0].val + std::string("'")));
        }
        
        std::shared_ptr<Operand> target = to_operand(tokens[2]);
        return Instruction(op, source, target);
    }
    throw AssemblerError(std::string("instruction '") + tokens[0].val +
                         std::string("' has bad operand count ") + std::to_string(operand_count));
}


void Assembler::write_nr(spg_register_t nr, int byte_count) {
    //note: this loop always writes big endian (on any architecture), which is what we want
    for (int byte_index = byte_count-1; byte_index >= 0; byte_index--) {
        uint8_t to_write = (uint8_t)((nr >> (byte_index*8)) & 0xFF);
        *cur_target = to_write;
        cur_target++;
        assert((size_t)(cur_target - target) < target_size);
    }
}


void Assembler::write_data(const std::vector<AssemblerToken>& tokens) {
    if (tokens.size() < 2) return;
    assert(tokens[0].tokenType == DATASPEC);
    int data_len = to_data_bytecount(tokens[0]);
    if (data_len > 0) {
        std::vector<AssemblerToken>::const_iterator it = tokens.begin();
        it++;
        for (; it != tokens.end(); it++) {
            const AssemblerToken& tok = *it;
            assert(tok.tokenType == DATA_NR);
            spg_register_t nr = to_nr(tok);
            write_nr(nr, data_len);
        }
        return;
    }
    assert(false);
    throw AssemblerError(std::string("unsupported data spec '" + tokens[0].val + std::string("'")));
}


void Assembler::write_instruction(const Instruction &instruction) {
    instruction.encode([this](uint8_t byte){
        *cur_target = byte;
        cur_target++;
        assert((size_t)(cur_target - target) < target_size);
    });
}


Operation Assembler::to_operation(const AssemblerToken &token) {
    assert(token.tokenType == OPERATION);
    bool is_op;
    Operation res = to_operation_internal(token.val);
    return res;
}


Operation Assembler::to_operation_internal(const std::string &op_str) {
    std::string base_op(op_str);
    auto to_upper = [] (char ch) { return toupper(ch); };
    std::transform(base_op.begin(), base_op.end(), base_op.begin(), to_upper);
    
    std::unordered_map<std::string, Operation>::const_iterator it = op_map.find(base_op);
    if (it != op_map.end()) {
        return it->second;
    }
    throw AssemblerError(std::string("Error parsing OP '") + op_str + std::string("') "));
}

uint8_t Assembler::reg_name_to_index(const std::string& reg_name) {
    assert(reg_name != "r");
    if (reg_name.empty()) throw AssemblerError("reg name empty");
    if (reg_name == "pc" || reg_name == "ip") return 0;
    if (reg_name == "sp") return 1;
    if (reg_name == "a") return 2;
    if (reg_name == "b") return 3;
    if (reg_name == "c") return 4;
    if (reg_name == "d") return 5;
    if (reg_name == "e") return 6;
    if (reg_name == "f") return 7;
    if (reg_name == "bp")
        throw AssemblerError(std::string("this simplified CPU has no register %bp"));
    throw AssemblerError(std::string("reg name invalid '")+reg_name+std::string("'"));
}


size_t Assembler::to_operand_bitsize(const AssemblerToken &token) {
    assert(token.tokenType == OPERAND);
    
    std::smatch sm;
    if (regex_search(token.val, sm, operand_address_regex)) {
        return 2 + sizeof(spg_addr_t)*8;
    }
    if (regex_search(token.val, sm, operand_mem_reg_regex)) {
        return 2 + 3 + 8;
    }
    if (regex_search(token.val, sm, operand_imm_regex)) {
        return 2 + sizeof(spg_register_t)*8;
    }
    if (regex_search(token.val, sm, operand_reg_regex)) {
        return 2 + 3;
    }
    throw AssemblerError(std::string("could not parse operand '")+token.val+std::string("'"));
}

std::shared_ptr<Operand> Assembler::to_operand(const AssemblerToken &token) {
    assert(token.tokenType == OPERAND);
    
    std::smatch sm;
    if (regex_search(token.val, sm, operand_address_regex)) {
        const std::string &addr = sm[1].str();
        typename std::map<std::string, spg_addr_t>::iterator it = labels.find(addr);
        spg_addr_t addr_val;
        if (it != labels.end()) {
            addr_val = (spg_addr_t) it->second;
        } else {
            std::map<std::string, spg_register_t>::iterator it2 = constants.find(addr);
            if (it2 != constants.end()) {
                addr_val = it2->second;
            } else {
                addr_val = (spg_addr_t) std::stoul(addr, nullptr, 0);
            }
        }
        
        return std::shared_ptr<Operand>(new AddressOperand(addr_val));
    }
    if (regex_search(token.val, sm, operand_mem_reg_regex)) {
        uint8_t reg_index = reg_name_to_index(sm[2].str());
        int8_t disp = sm[1].str().empty() ? 0 : (int8_t) std::stoul(sm[1].str(), nullptr, 0);
        return std::shared_ptr<Operand>(new MemRegisterOperand(reg_index, disp));
    }
    if (regex_search(token.val, sm, operand_imm_regex)) {
        const std::string &imm = sm[1].str();
        typename std::map<std::string, spg_addr_t>::iterator it = labels.find(imm);
        spg_register_t imm_val;
        if (it != labels.end()) {
            imm_val = (spg_register_t) it->second;
        } else {
            std::map<std::string, spg_register_t>::iterator it2 = constants.find(imm);
            if (it2 != constants.end()) {
                imm_val = it2->second;
            } else {
                try {
                    imm_val = (spg_register_t) std::stoul(imm, nullptr, 0);
                } catch (std::invalid_argument& e) {
                    throw AssemblerError(std::string("could not parse operand '")+token.val+std::string("'"));
                } catch (std::out_of_range& e) {
                    throw AssemblerError(std::string("operand '")+token.val+std::string("' is out of range"));
                }
            }
        }
        return std::shared_ptr<Operand>(new ImmediateOperand(imm_val));
    }
    if (regex_search(token.val, sm, operand_reg_regex)) {
        uint8_t reg_index = reg_name_to_index(sm[1].str());
        return std::shared_ptr<Operand>(new RegisterOperand(reg_index));
    }
    throw AssemblerError(std::string("could not parse operand '")+token.val+std::string("'"));
}

std::vector<uint8_t> assemble(const std::vector<std::string> &asmLinesIn, spg_addr_t start_addr_code) {
    std::vector<std::string> allLines;
    allLines.push_back(std::string(".equ DECISION_FORWARD ") + std::to_string(DECISION_FORWARD));
//    lines.push_back(std::string(".equ DECISION_BACK ") + std::to_string(BACK));
    allLines.push_back(std::string(".equ DECISION_ATTACH ") + std::to_string(DECISION_ATTACH));
    allLines.push_back(std::string(".equ DECISION_DROP ") + std::to_string(DECISION_DROP));
    allLines.push_back(std::string(".equ DECISION_TURN_LEFT ") + std::to_string(DECISION_TURN_LEFT));
    allLines.push_back(std::string(".equ DECISION_TURN_RIGHT ") + std::to_string(DECISION_TURN_RIGHT));
    allLines.push_back(std::string(".equ DECISION_IDLE ") + std::to_string(DECISION_IDLE));

    allLines.push_back(std::string(".equ IO_DECISION_START ") + std::to_string(IO_DECISION_START));
    allLines.push_back(std::string(".equ IO_VISION_START ") + std::to_string(IO_VISION_START));

    allLines.push_back(std::string(".equ SENSOR_POSITION_COUNT ") + std::to_string(SENSOR_POSITION_COUNT));
    //sensor positions start immediately at IO_VISION_START
    allLines.push_back(std::string(".equ SENSOR_POSITION_0_ADDR ") + std::to_string(IO_VISION_START));
    //crate count is always after sensor positions
    allLines.push_back(std::string(".equ SENSOR_CRATE_COUNT_ADDR ") + std::to_string(IO_VISION_START + SENSOR_POSITION_COUNT));

    allLines.push_back(std::string(".equ SENSE_ROAD ") + std::to_string(SENSE_ROAD));
    allLines.push_back(std::string(".equ SENSE_EXIT ") + std::to_string(SENSE_EXIT));
    allLines.push_back(std::string(".equ SENSE_MARKER_A ") + std::to_string(SENSE_MARKER_A));
    allLines.push_back(std::string(".equ SENSE_MARKER_B ") + std::to_string(SENSE_MARKER_B));
    allLines.push_back(std::string(".equ SENSE_MARKER_C ") + std::to_string(SENSE_MARKER_C));
    allLines.push_back(std::string(".equ SENSE_MARKER_D ") + std::to_string(SENSE_MARKER_D));
    allLines.push_back(std::string(".equ SENSE_FILTER ") + std::to_string(SENSE_FILTER));
    allLines.push_back(std::string(".equ SENSE_CRATE ") + std::to_string(SENSE_CRATE));

//    std::cout << "Adding assembler preamble (" << allLines.size() << " lines)" << std::endl;
//    for (const std::string &line: allLines) {
//        std::cout << "Adding assemble preamble: " << line << std::endl;
//    }
    for (const std::string &line: asmLinesIn) {
        allLines.push_back(line);
    }

    uint8_t exe[1024];
    Assembler assembler(start_addr_code);
    size_t codeByteSize = assembler.assemble(allLines, exe, sizeof(exe));
    return {exe, exe + codeByteSize};
}

#pragma clang diagnostic pop

