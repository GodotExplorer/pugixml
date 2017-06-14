#ifndef __GODOT_EXPLORER_PUGIXML_EXPORTER_H__
#define __GODOT_EXPLORER_PUGIXML_EXPORTER_H__

#include <core/os/file_access.h>
#include <core/reference.h>

#include "pugixml/pugixml.hpp"

// pugi string to godot string
String str_p2g(const char* str);

class XMLAttribute : public Reference {
	OBJ_TYPE(XMLAttribute, Reference);
	friend class XMLNode;

protected:
	pugi::xml_attribute *m_pAttr;
	static void _bind_methods();
	void _set_native_attr(const pugi::xml_attribute &native_attr);

public:
	XMLAttribute();
	virtual ~XMLAttribute();
	static Ref<XMLAttribute> create(const pugi::xml_attribute& native_attr);

	bool empty() const { return m_pAttr->empty(); }
	String name() const { return str_p2g(m_pAttr->name()); }
	void set_name(const String &name) { m_pAttr->set_name(name.utf8()); }
	String value() const { return str_p2g(m_pAttr->value()); }
	void set_value(const Variant &value) { m_pAttr->set_value(String(value).utf8()); }
};

class XMLNode;
class XMLText : public Reference {
	OBJ_TYPE(XMLText, Reference);
	friend class XMLNode;

protected:
	pugi::xml_text *m_pText;
	static void _bind_methods();
	void _set_native_text(const pugi::xml_text &native_text);

public:
	XMLText();
	virtual ~XMLText();
	static Ref<XMLText> create(const pugi::xml_text& native_text);

	bool empty() const { return m_pText->empty(); }
	String get_value() const { return str_p2g(m_pText->get()); }
	void set_value(const String &text) { m_pText->set(text.utf8()); }
	Ref<XMLNode> data();
};

class XMLNode : public Reference {
	OBJ_TYPE(XMLNode, Reference);

protected:
	static void _bind_methods();
	pugi::xml_node *m_pNode;

	void _set_pugi_node(const pugi::xml_node &p_node);

public:
	enum XMLNodeType {
		NODE_NULL, // Empty (null) node handle
		NODE_DOCUMENT, // A document tree's absolute root
		NODE_ELEMENT, // Element tag, i.e. '<node/>'
		NODE_PCDATA, // Plain character data, i.e. 'text'
		NODE_CDDATA, // Character data, i.e. '<![CDATA[text]]>'
		NODE_COMMENT, // Comment tag, i.e. '<!-- text -->'
		NODE_PI, // Processing instruction, i.e. '<?name?>'
		NODE_DECLARATION, // Document declaration, i.e. '<?xml version="1.0"?>'
		NODE_DOCTYPE // Document type declaration, i.e. '<!DOCTYPE doc>'
	};
	XMLNode();
	virtual ~XMLNode();
	static Ref<XMLNode> create(const pugi::xml_node &p_node);

	bool empty() const { return !m_pNode || m_pNode->empty(); }
	XMLNodeType type() const { return XMLNodeType(int(m_pNode->type())); }
	String name() const { return str_p2g(m_pNode->name()); }
	String value() const { return str_p2g(m_pNode->value()); }

	Ref<XMLAttribute> first_attribute() const { return XMLAttribute::create(m_pNode->first_attribute());}
	Ref<XMLAttribute> last_attribute() const { return XMLAttribute::create(m_pNode->last_attribute()); }

	Ref<XMLNode> first_child() const { return create(m_pNode->first_child());}
	Ref<XMLNode> last_child() const { return create(m_pNode->last_child()); }

	Ref<XMLNode> next_sibling() const { return create(m_pNode->next_sibling()); }
	Ref<XMLNode> previous_sibling() const { return create(m_pNode->previous_sibling()); }

	Ref<XMLNode> parent() const { return create(m_pNode->parent()); }
	Ref<XMLNode> root() const { return create(m_pNode->root()); }
	Ref<XMLText> text() const { return XMLText::create(m_pNode->text()); }

	Ref<XMLNode> child(const String &name) const { return create(m_pNode->child(name.utf8())); }
	Ref<XMLAttribute> attribute(const String &name) const { return XMLAttribute::create(m_pNode->attribute(name.utf8())); }
	Ref<XMLNode>  next_element(const String& name) const { return create(m_pNode->next_sibling(name.utf8())); }
	Ref<XMLNode>  previous_element(const String& name) const { return create(m_pNode->previous_sibling(name.utf8())); }

	String child_value() const { return str_p2g(m_pNode->child_value()); }
	String element_value(const String& name) const { return str_p2g(m_pNode->child_value(name.utf8())); }

	void set_name(const String &rhs) { m_pNode->set_name(rhs.utf8()); }
	void set_value(const String &rhs) { m_pNode->set_value(rhs.utf8()); }

	Ref<XMLAttribute> append_attribute(const String &name) { return XMLAttribute::create(m_pNode->append_attribute(name.utf8())); }
	Ref<XMLAttribute> prepend_attribute(const String &name) { return XMLAttribute::create(m_pNode->prepend_attribute(name.utf8())); }
	Ref<XMLAttribute> insert_attribute_after(const String &name, const Ref<XMLAttribute>& attr) { return XMLAttribute::create(m_pNode->insert_attribute_after(name.utf8(), *(attr->m_pAttr))); }
	Ref<XMLAttribute> insert_attribute_before(const String &name, const Ref<XMLAttribute>& attr) { return XMLAttribute::create(m_pNode->insert_attribute_before(name.utf8(), *(attr->m_pAttr))); }

	Ref<XMLNode> append_child(XMLNodeType type) { return create(m_pNode->append_child(pugi::xml_node_type(type))); }
	Ref<XMLNode> prepend_child(XMLNodeType type) { return create(m_pNode->prepend_child(pugi::xml_node_type(type))); }
	Ref<XMLNode> insert_child_after(XMLNodeType type, const Ref<XMLNode>& node) { return create(m_pNode->insert_child_after(pugi::xml_node_type(type), *(node->m_pNode))); }
	Ref<XMLNode> insert_child_before(XMLNodeType type, const Ref<XMLNode>& node) { return create(m_pNode->insert_child_before(pugi::xml_node_type(type), *(node->m_pNode))); }

	Ref<XMLNode> append_element(const String &name) { return create(m_pNode->append_child(name.utf8())); }
	Ref<XMLNode> prepend_element(const String &name) { return create(m_pNode->prepend_child(name.utf8())); }
	Ref<XMLNode> insert_element_after(const String &name, const Ref<XMLNode>& node) { return create(m_pNode->insert_child_after(name.utf8(), *(node->m_pNode))); }
	Ref<XMLNode> insert_element_before(const String &name, const Ref<XMLNode>& node) { return create(m_pNode->insert_child_before(name.utf8(), *(node->m_pNode))); }

	// Add a copy of the specified node as a child. Returns added node, or empty node on errors.
	Ref<XMLNode> append_copy(const Ref<XMLNode>& proto) { return create(m_pNode->append_copy(*(proto->m_pNode))); }
	Ref<XMLNode> prepend_copy(const Ref<XMLNode>& proto) { return create(m_pNode->prepend_copy(*(proto->m_pNode))); }
	Ref<XMLNode> insert_copy_after(const Ref<XMLNode>& proto, const Ref<XMLNode>& node) { return create(m_pNode->insert_copy_after(*(proto->m_pNode), *(node->m_pNode))); }
	Ref<XMLNode> insert_copy_before(const Ref<XMLNode>& proto, const Ref<XMLNode>& node) { return create(m_pNode->insert_copy_before(*(proto->m_pNode), *(node->m_pNode))); }

	// Move the specified node to become a child of this node. Returns moved node, or empty node on errors.
	Ref<XMLNode> append_move(const Ref<XMLNode>& moved) { return create(m_pNode->append_move(*(moved->m_pNode))); }
	Ref<XMLNode> prepend_move(const Ref<XMLNode>& moved) { return create(m_pNode->prepend_move(*(moved->m_pNode))); }
	Ref<XMLNode> insert_move_after(const Ref<XMLNode>& moved, const Ref<XMLNode>& node) { return create(m_pNode->insert_move_after(*(moved->m_pNode), *(node->m_pNode))); }
	Ref<XMLNode> insert_move_before(const Ref<XMLNode>& moved, const Ref<XMLNode>& node) { return create(m_pNode->insert_move_before(*(moved->m_pNode), *(node->m_pNode))); }

	// Remove specified attribute
	bool remove_attribute(const Ref<XMLAttribute>& a) { return m_pNode->remove_attribute(*(a->m_pAttr)); }
	bool remove_attribute_by_name(const String &name) { return m_pNode->remove_attribute(name.utf8()); }

	// Remove specified child
	bool remove_child(const Ref<XMLNode>& n) { return m_pNode->remove_child(*(n->m_pNode)); }
	bool remove_child_by_name(const String &name) { return m_pNode->remove_child(name.utf8()); }

	// Find child node by attribute name/value
	Ref<XMLNode> first_element_by_path(const String& path) const { return create(m_pNode->first_element_by_path(path.utf8())); }
	Ref<XMLNode> find_child_by_attribute(const String &attr_name, const String &attr_value, const String &ele_name) const;
	Vector<Variant> get_children() const;
	Vector<Variant> get_elements(const String& name) const;
	Vector<Variant> get_attributes() const;

};

class XMLDocument : public XMLNode {
	OBJ_TYPE(XMLDocument, XMLNode);

protected:
	static void _bind_methods();

public:
	XMLDocument();
	virtual ~XMLDocument();

	Error load_string(const String &contents);
	Error load_file(const String &path);
	String save_string(const String &indent="\t");
	Error save_file(const String &path, const String &indent="\t");
};

VARIANT_ENUM_CAST(XMLNode::XMLNodeType);

#endif // __GODOT_EXPLORER_PUGIXML_EXPORTER_H__
