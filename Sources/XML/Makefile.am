AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclan42XML.la

libclan42XML_la_SOURCES = \
XML/xml_writer.cpp \
XML/dom_exception.cpp \
XML/dom_entity.cpp \
XML/xpath_evaluator.cpp \
XML/dom_document.cpp \
XML/dom_cdata_section.cpp \
XML/dom_text.cpp \
XML/dom_comment.cpp \
XML/dom_document_type.cpp \
XML/dom_notation.cpp \
XML/dom_document_generic.cpp \
XML/dom_processing_instruction.cpp \
XML/dom_named_node_map.cpp \
XML/dom_entity_reference.cpp \
XML/dom_element.cpp \
XML/xpath_object.cpp \
XML/dom_node.cpp \
XML/xpath_exception.cpp \
XML/dom_character_data.cpp \
XML/dom_attr.cpp \
XML/dom_implementation.cpp \
XML/xml_tokenizer.cpp \
XML/dom_node_list.cpp \
XML/dom_document_fragment.cpp \
XML/xpath_evaluator_impl.cpp \
Resources/xml_resource_node.cpp \
Resources/xml_resource_manager.cpp \
Resources/xml_resource_document.cpp \
SoundResources/XML/xml_sound_cache.cpp \
SoundResources/XML/soundbuffer_xml.cpp \
DisplayResources/XML/xml_display_cache.cpp \
DisplayResources/XML/cursor_description_xml.cpp \
DisplayResources/XML/image_xml.cpp \
DisplayResources/XML/program_object_xml.cpp \
DisplayResources/XML/shader_object_xml.cpp \
DisplayResources/XML/sprite_xml.cpp \
DisplayResources/XML/texture_xml.cpp \
DisplayResources/XML/font_xml.cpp

libclan42XML_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanXML)

libclan42XML_la_CXXFLAGS=$(clanXML_CXXFLAGS) $(extra_CFLAGS_clanXML)

# EOF #
