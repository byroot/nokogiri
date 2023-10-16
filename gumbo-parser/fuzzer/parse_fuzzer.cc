#include "../src/nokogiri_gumbo.h"
#include <stdint.h>

void SanityCheckPointers(
    const char* input, size_t input_length, const GumboNode* node, int depth) {
  if (node->type == GUMBO_NODE_DOCUMENT || depth > 400) {
    return;
  }
  if (node->type == GUMBO_NODE_ELEMENT) {
    const GumboElement* element = &node->v.element;
    const GumboVector* children = &element->children;
    for (unsigned int i = 0; i < children->length; ++i) {
      const GumboNode* child = static_cast<const GumboNode*>(children->data[i]);
      SanityCheckPointers(input, input_length, child, depth + 1);
    }
  } else {
    const GumboText* text = &node->v.text;
  }
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 10)
  {
      return 0;
  }
  
  GumboOptions options = kGumboDefaultOptions;
  GumboOutput* output;
  GumboNode* root;

  output = gumbo_parse_with_options(&options, (char*)data, size);
  root = output->document;
  SanityCheckPointers((char*)data, size, output->root, 0);
    
  if (output) {
    gumbo_destroy_output(output);
  }

	return 0;
}