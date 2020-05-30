/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include "mm2cpp.h"

#include <ostream>

#include <util/std_code.h>

class mm2cppt
{
public:
  explicit mm2cppt(std::ostream &_out):out(_out)
  {
  }

  irep_idt model_name;
  void operator()(const irept &);

protected:
  std::ostream &out;
  typedef std::map<irep_idt, exprt> let_valuest;
  let_valuest let_values;

  static std::string text2c(const irep_idt &src);
  void instruction2cpp(const codet &code, unsigned indent);
  void check_acyclic(const exprt &, unsigned indent);
};

std::string mm2cppt::text2c(const irep_idt &src)
{
  std::string result;
  result.reserve(src.size());

  for(unsigned i=0; i<src.size(); i++)
  {
    char ch=src[i];
    if(isalnum(ch))
      result+=ch;
    else
      result+='_';
  }

  return result;
}

void mm2cppt::check_acyclic(const exprt &expr, unsigned indent)
{
  if(expr.id()==ID_symbol)
  {
    const irep_idt &identifier=expr.get(ID_identifier);
    let_valuest::const_iterator v_it=let_values.find(identifier);
    if(v_it!=let_values.end())
      check_acyclic(v_it->second, indent);
    else if(identifier=="po")
    {
    }
    else if(identifier=="rf")
    {
    }
    else if(identifier=="co")
    {
    }
    else if(identifier=="loc")
    {
    }
    else if(identifier=="int")
    {
    }
    else if(identifier=="ext")
    {
    }
    else if(identifier=="id")
    {
    }
    else if(identifier=="R")
    {
    }
    else if(identifier=="W")
    {
    }
    else if(identifier=="F")
    {
    }
    else if(identifier=="B")
    {
    }
    else if(identifier=="RMW")
    {
    }
    else if(identifier=="rmw")
    {
    }
    else if(identifier=="IW")
    {
    }
    else if(identifier=="FW")
    {
    }
    else if(identifier=="M")
    {
    }
    else if(identifier=="addr")
    {
    }
    else if(identifier=="data")
    {
    }
    else if(identifier=="ctrl")
    {
    }
    else
    {
      throw expr.source_location().as_string()+
            ": unknown identifier `"+id2string(identifier)+"'";
    }
  }
  else if(expr.id()==ID_union)
  {
    assert(expr.operands().size()==2);
    check_acyclic(expr.op0(), indent);
    check_acyclic(expr.op1(), indent);
  }
  else
    throw "acyclic cannot do "+expr.id_string();
}

void mm2cppt::instruction2cpp(const codet &code, unsigned indent)
{
  const irep_idt &statement=code.get_statement();

  if(statement==ID_block)
  {
    forall_operands(it, code)
    {
      instruction2cpp(to_code(*it), indent+2);
    }
  }
  else if(statement==ID_let)
  {
    assert(code.operands().size()==1);
    const exprt &binding_list=code.op0();
    forall_operands(it, binding_list)
    {
      if(it->id()=="valbinding")
      {
        assert(it->operands().size()==2);
        if(it->op0().id()==ID_symbol)
        {
          irep_idt identifier=it->op0().get(ID_identifier);
          let_values[identifier]=it->op1();
        }
        else
          throw "cannot do tuple valbinding";
      }
      else if(it->id()=="funbinding")
      {
        throw "cannot do funbinding";
      }
      else
        throw "unknown let binding";
    }
  }
  else if(statement=="check")
  {
    assert(code.operands().size()==3);
    if(code.op0().id()=="acyclic")
    {
      check_acyclic(code.op1(), indent);
    }
    else
    {
      throw "can only do 'acyclic'";
    }
  }
}

void mm2cppt::operator()(const irept &instruction)
{
  out << "// Generated by mmcc\n";
  out << "// Model: " << model_name << '\n';
  out << '\n';

  out << "class memory_model_" << text2c(model_name) << "t\n";
  out << "{\n";
  out << "public:\n";
  out << "  void operator()();\n";
  out << "};\n";
  out << '\n';

  out << "void memory_model_" << text2c(model_name) << "t::operator()()\n";
  out << "{\n";
  instruction2cpp(to_code(static_cast<const exprt &>(instruction)), 0);
  out << "}\n";
  out << '\n';
}

void mm2cpp(
  const irep_idt &model_name,
  const irept &instruction,
  std::ostream &out)
{
  mm2cppt mm2cpp(out);
  mm2cpp.model_name=model_name;
  mm2cpp(instruction);
}