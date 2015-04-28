#include "regex.hpp"

__MYLO_NAMESPACE_BEGIN

regex::state::state(enum types type) {
  init();
  m_type = type;
  m_literal = 0;
  m_charset = NULL;
  m_max = 1;
  m_min = 1;
}

regex::state::state(char literal, bool negate) {
  init();
  m_type = LITERAL;
  m_literal = literal;
  m_charset = NULL;
  m_negate = negate;
}

regex::state::state(char *charset, bool negate) {
  init();
  m_type = CHARSET;
  m_literal = 0;
  if(m_type == CHARSET) {
    size_t length = strlen(charset);
    m_charset = (char *)malloc(length+1);
    memcpy(m_charset, charset, length);
    m_charset[length] = '\0';
  }
  m_negate = negate;
}

regex::state::~state(void) {
  if(m_type == CHARSET && m_charset) {
    free(m_charset);
  }
}

void regex::state::init(void) {
  m_root = NULL;
  m_count = 0;
  m_transitions = NULL;
  m_negate = false;
  m_max = 0;
  m_min = 0;
  m_greedy = true;
  m_subexp_id = 0;
}

regex::state *regex::state::transition(regex::state *next) {
  int size = (m_count + 2) * sizeof(state *);
  m_transitions = (state **)realloc(m_transitions, (++m_count + 1) * sizeof(state *));
  m_transitions[m_count-1] = next;
  return(next);
}

void regex::state::boundary(bool greedy, int max, int min) {
  m_greedy = greedy;
  m_max = max;
  m_min = min;
}


regex::stack::stack(void) {
  m_data = NULL;
  m_size = 0;
  m_count = 0;
}

regex::stack::~stack(void) {
  for(int i=0; i < m_count; i++) {
    delete m_data[i];
  }
  if(m_data) {
    free(m_data);
  }
}

regex::node *regex::stack::push(regex::node *value) {
  m_size += 1;
  m_count += 1;
  m_data = (node **)realloc(m_data, (m_count+1) * sizeof(node *));
  m_data[m_count-1] = value;
  m_data[m_count] = NULL;
  return(value);
}

regex::node *regex::stack::pop(void) {
  if(m_count > 0) {
    node *top = m_data[--m_count];
    m_data[m_count] = NULL;
    return(top);
  }
  else {
    return(NULL);
  }
}

regex::node *regex::stack::last(void) {
  if(m_count > 0) {
    return(m_data[m_count-1]);
  }
  else {
    return(NULL);
  }
}

__MYLO_DLL_EXPORT regex::regex(const char *parameter) {
  m_root = new state(START);

  m_subexp_count = 0;

  m_anchor_front = false;
  m_anchor_back = false;

  if(parameter != NULL) {
    compile(parameter);
  }
}

void regex::cleanup(state *start) {
  if(start == NULL) {
    start = m_root;
  }

  state *current = start;

  if(current->m_type == SUBEXP) {
    cleanup(current->m_root);
  }
  for(int i=0; i<current->m_count; i++) {
    cleanup(current->m_transitions[i]);
  }
  free(current->m_transitions);
  current->m_transitions = NULL;
  delete current;
}

__MYLO_DLL_EXPORT int regex::compile(const char *parameter, state *root) {
  if(root == NULL) {
    root = m_root;
  }
  
  state *start = root;
  state *current = root;
  
  start->m_subexp_id = ++m_subexp_count - 1;
  
  int index = 0;
  char charset[__RE_BUFFERSIZE];
  
  while(parameter[index]) {
    if(__RE_CHARSET_ESCAPE[parameter[index]] == __RE_CHARSET_VALUE && __RE_CHARSET_CONTROL[parameter[index+1]] == __RE_CHARSET_VALUE) {
      switch(parameter[index+1]) {
      case('w'):   current = current->transition(new state(__RE_CHARSET_ALPHANUMERIC));        break;
      case('W'):   current = current->transition(new state(__RE_CHARSET_ALPHANUMERIC, true));  break;
      case('d'):   current = current->transition(new state(__RE_CHARSET_NUMERIC));             break;
      case('D'):   current = current->transition(new state(__RE_CHARSET_NUMERIC, true));       break;
      case('s'):   current = current->transition(new state(__RE_CHARSET_SPACE));               break;
      case('S'):   current = current->transition(new state(__RE_CHARSET_SPACE, true));         break;
      };
      index += 1;
    }
    
    else if(__RE_CHARSET_ESCAPE[parameter[index]] == __RE_CHARSET_VALUE) {
      current = current->transition(new state(parameter[++index]));
    }
    
    else if(parameter[index] == '.') {
      current = current->transition(new state(__RE_CHARSET_ANY));
    }
    
    else if(parameter[index] == '[') {
      bool negate  = false;
      bool collate = false;
      bool equivalence = false;
      bool cc = false;
      switch(parameter[index+1]) {
      case('^'):
        negate = true;
        index++;
        break;
      case('.'):
        collate = true;
        index++;
        break;
      case('='):
        equivalence = true;
        index++;
        break;
      case(':'):
        cc = true;
        index++;
        break;
      };
      memset(charset, __RE_CHARSET_UNSET, __RE_BUFFERSIZE);
      while(parameter[index] != ']') {
        charset[parameter[index++]] = __RE_CHARSET_VALUE;
      }
      charset[__RE_BUFFERSIZE-1] = '\0';
      current = current->transition(new state(charset, negate));
    }

    else if(parameter[index] == '(') {
      current = current->transition(new state(SUBEXP));
      current->m_root = new state(START);
      current->m_root->m_root = current;
      ++index += compile(parameter + index, current->m_root);
    }

    else if(parameter[index] == ')') {
      current = current->transition(new state(STOP));
      current->m_subexp_id = start->m_subexp_id;
      current->m_root = start->m_root;
      return(index);
    }

    else if(parameter[index] == '|') {
      current = current->transition(new state(STOP));
      current->m_subexp_id = start->m_subexp_id;
      current->m_root = start->m_root;
      current = start;
    }

    else if(parameter[index] == '^' && index == 0) {
      m_anchor_front = true;
      index++;
      continue;
    }

    else if(parameter[index] == '$' && parameter[index+1] == '\0') {
      m_anchor_back = true;
      index++;
      continue;
    }

    else {
      current = current->transition(new state(parameter[index]));
    }

    int min = 1;
    int max = 1;
    char min_str[6];
    char max_str[6];
    bool next;
    char *endptr;
    bool greedy = true;

    switch(parameter[index+1]) {
    case('+'):
      min = 1;
      max = -1;
      index++;
      if(parameter[index+1] == '?') {
        greedy = false;
        index++;
      }
      break;

    case('*'):
      min = 0;
      max = -1;
      index++;
      if(parameter[index+1] == '?') {
        greedy = false;
        index++;
      }
      break;

    case('?'):
      min = 0;
      max = 1;
      index++;
      if(parameter[index+1] == '?') {
        greedy = false;
        index++;
      }
      break;

    case('{'):
      index += 2;
      next = false;
      memset(min_str, 0, 6);
      memset(max_str, 0, 6);
      min = -1;
      max = -1;
      while(parameter[index] != '}') {
        if(parameter[index] == ',') {
          next = true;
        }
        else {
          next ? max_str[strlen(max_str)] = parameter[index] : min_str[strlen(min_str)] = parameter[index];
        }
        index++;
      }
      if(strlen(min_str)) {
        min = strtoul(min_str, &endptr, 10);
        if(strlen(max_str)) {
          max = strtoul(max_str, &endptr, 10);
        }
        else {
          if(!next) { max = min; }
        }
      }
      if(parameter[index+1] == '?') {
        greedy = false;
        index++;
      }
      break;
    };
    current->boundary(greedy, max, min);
    if(parameter[index]) { index++; }
  }

  current->transition(new state(STOP));

  return(index);
}

__MYLO_DLL_EXPORT regex::stack *regex::execute(const char *reference) {
  int index=0;
  int attempt=0;
  state *current = m_root;

  stack *history_stack = new stack;
  stack *entry_stack   = new stack;

  while(1) {
    for(int i=0; i<history_stack->m_count; i++) {
      if(history_stack->m_data[i]->m_current->m_type == START) {
        continue;
      }
      if(history_stack->m_data[i]->m_current->m_type == STOP) {
        continue;
      }
      if(history_stack->m_data[i]->m_current->m_type == LITERAL) {
        continue;
      }
      if(history_stack->m_data[i]->m_current->m_type == CHARSET) {
        continue;
      }
      if(history_stack->m_data[i]->m_current->m_type == SUBEXP) {
        continue;
      }
    }
      
    bool match = false;
    switch(current->m_type) {
      case(START):
        //DEBUG<<"START"<<std::endl;
        match = true;
        break;
        
      case(STOP):
        //DEBUG<<"STOP"<<std::endl;
        if(current->m_root) {
          history_stack->push(new node(current, index));
          current = current->m_root;
          match = true;
        }
        else {
          history_stack->push(new node(current, index));
          if(m_anchor_back) {
            if(reference[index] == '\0') {
              delete entry_stack;
              return(history_stack);
            }
          }
          else {
            delete entry_stack;
            return(history_stack);
          }
        }
        break;
        
      case(LITERAL):
        if(reference[index] == current->m_literal) {
          //DEBUG<<"LITERAL: match '"<<reference[index]<<"'"<<std::endl;
          match = true;
          index++;
        }
        else {
          //DEBUG<<"LITERAL"<<std::endl;
        }
        break;
        
      case(CHARSET):
        if(current->m_charset[reference[index]] == __RE_CHARSET_VALUE) {
          match = !current->m_negate;
        }
        else {
          if(reference[index]) {
            match = current->m_negate;
          }
        }
        if(match) {
          //DEBUG<<"CHARSET: match '"<<reference[index]<<"'"<<std::endl;
          index++;
        }
        else {
          //DEBUG<<"CHARSET"<<std::endl;
        }
        break;
        
      case(SUBEXP):
        //DEBUG<<"SUBEXP"<<std::endl;
        current = current->m_root;
        continue;
    };
    
    node *last_entry = entry_stack->last();
    int count = 0;
    int transition = 0;
    if(last_entry && last_entry->m_current == current) {
      count = last_entry->m_count;
      transition = last_entry->m_transition;
    }
    entry_stack->push(new node(current, index, count+1, transition));
    
    if(match) {
      history_stack->push(new node(entry_stack->last()));
      
      if(current->m_greedy) {
        //DEBUG<<"... greedy, max "<<current->m_max<<std::endl;
        if(current->m_max != -1 && count+1 >= current->m_max) {
          while(entry_stack->last() && entry_stack->last()->m_current == current) {
            delete entry_stack->pop();
          }
          current = current->m_transitions[0];
        }
      }
      else {
        if(count+1 >= current->m_min) {
          while(entry_stack->last() && entry_stack->last()->m_current == current) {
            delete entry_stack->pop();
          }
          current = current->m_transitions[0];
        }
      }
    }
    else {
      while(entry_stack->last() && entry_stack->last()->m_current == current) {
        delete entry_stack->pop();
      }
      
      if(count >= current->m_min) {
        current = current->m_transitions[0];
        continue;
      }
      
      current = NULL;
      node *backtrack = history_stack->last();
      while(backtrack) {
        if(backtrack == entry_stack->last()) {
          if(backtrack->m_count >= backtrack->m_current->m_min) {
            current = backtrack->m_current->m_transitions[0];
            index = backtrack->m_index;
            delete entry_stack->pop();
            break;
          }
        }
        if(backtrack->m_transition+1 < backtrack->m_current->m_count) {
          backtrack->m_transition += 1;
          current = backtrack->m_current->m_transitions[backtrack->m_transition];
          index = backtrack->m_index;
          break;
        }
        if(backtrack->m_current->m_type == LITERAL || backtrack->m_current->m_type == CHARSET) {
          if(backtrack->m_current->m_greedy) {
            if(backtrack->m_count-1 >= backtrack->m_current->m_min) {
              delete history_stack->pop();
              backtrack = history_stack->last();
              if(backtrack->m_current->m_type != START) {
                current = backtrack->m_current->m_transitions[backtrack->m_transition];
                index = backtrack->m_index;
                break;
              }
            }
          }
          else {
            if(backtrack->m_current->m_max == -1 || backtrack->m_count-1 < backtrack->m_current->m_max) {
              if(reference[backtrack->m_index]) {
                current = backtrack->m_current;
                index = backtrack->m_index;
                break;
              }
              else {
                while(history_stack->last() && history_stack->last()->m_current == backtrack->m_current) {
                  delete history_stack->pop();
                }
                if(!history_stack->last()) {
                  current = NULL;
                }
                else {
                  current = history_stack->last()->m_current;
                }
              }
            }
          }
        }
        if(backtrack->m_current->m_type == START &&
           backtrack->m_current->m_root != NULL &&
           backtrack->m_current->m_root->m_min == 0)
        {
          current = backtrack->m_current->m_root->m_transitions[0];
          index = backtrack->m_index;
          delete history_stack->pop();
          break;
        }

        delete history_stack->pop();
        backtrack = history_stack->last();
      }
      if(!current) {
        if(reference[++attempt] && !m_anchor_front) {
          index = attempt;
          current = m_root;
          while(history_stack->last()) {
            delete history_stack->pop();
          }
          while(entry_stack->last()) {
            delete entry_stack->pop();
          }
        }
        else {
          delete entry_stack;
          delete history_stack;
          return(NULL);
        }
      }
    }
  }

}

__MYLO_DLL_EXPORT my::vector< my::string > regex::match(const char *reference) {
  my::vector< my::string > list;

  if(reference == NULL) {
    return(list);
  }

  int *starts = (int *)malloc((m_subexp_count+1) * sizeof(int));

  stack *history_stack = execute(reference);
  if(history_stack) {
    for(int i=0; i<history_stack->m_count; i++) {
      if(history_stack->m_data[i]->m_current->m_type == START) {
        int index = history_stack->m_data[i]->m_current->m_subexp_id;
        starts[index] = history_stack->m_data[i]->m_index;
      }
      if(history_stack->m_data[i]->m_current->m_type == STOP) {
        int index = history_stack->m_data[i]->m_current->m_subexp_id;
        int front = starts[index];
        int back  = history_stack->m_data[i]->m_index;
        int size  = back - front;

        char *buffer = (char *)malloc((size + 1) * sizeof(char));
        buffer[0] = '\0';
        if(size) {
          memcpy(buffer, reference + front, size);
          buffer[size] = '\0';
        }
        //DEBUG<<"index("<<index<<") = '"<<buffer<<"'"<<std::endl;
        list[index] = buffer;
        
        free(buffer);
      }
    }
    delete history_stack;
  }

  free(starts);

  return(list);
}

__MYLO_DLL_EXPORT my::vector< my::pair<int, int> > regex::detailed_match(const char *reference) {
  my::vector< my::pair<int, int> > list;

  if(reference == NULL) {
    return(list);
  }

  int *starts = (int *)malloc((m_subexp_count+1) * sizeof(int));

  stack *history_stack = execute(reference);
  if(history_stack) {
    for(int i=0; i<history_stack->m_count; i++) {
      if(history_stack->m_data[i]->m_current->m_type == START) {
        int index = history_stack->m_data[i]->m_current->m_subexp_id;
        starts[index] = history_stack->m_data[i]->m_index;
      }
      if(history_stack->m_data[i]->m_current->m_type == STOP) {
        int index = history_stack->m_data[i]->m_current->m_subexp_id;
        int front = starts[index];
        int back  = history_stack->m_data[i]->m_index;
        int size  = back - front;

        char *buffer = (char *)malloc((size + 1) * sizeof(char));
        if(size) {
          memcpy(buffer, reference + front, size);
          buffer[size] = '\0';
        }
        
        list[index].first = front;
        list[index].second = back;

        free(buffer);
      }
    }
    delete history_stack;
  }

  free(starts);

  return(list);
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
