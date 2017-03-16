#include "pugixml_exporter.h"
#include <sstream>

using namespace  pugi;

XMLNode::XMLNode()
	: m_pNode(NULL) {
	pugi::xml_node n;
	_set_pugi_node(n);
//	print_line("Contructing XMLNode");
}

XMLNode::~XMLNode() {
	if (m_pNode)
		delete m_pNode;
	m_pNode = NULL;
//	print_line("Destructing XMLNode");
}

void XMLNode::_set_pugi_node(const xml_node &p_node) {
    if(&p_node != m_pNode) {
        if(m_pNode)
            delete m_pNode;
        m_pNode = new pugi::xml_node(p_node);
    }
}

Ref<XMLNode> XMLNode::create(const xml_node &p_node) {
    Ref<XMLNode> node = memnew(XMLNode);
    node->_set_pugi_node(p_node);
    return node;
}

Ref<XMLNode> XMLNode::find_child_by_attribute(const String &attr_name, const String &attr_value, const String &ele_name) const {
	if(!attr_value.empty())
		return create(m_pNode->find_child_by_attribute(ele_name.utf8(), attr_name.utf8(), attr_value.utf8()));
	else
		return create(m_pNode->find_child_by_attribute(attr_name.utf8(), attr_name.utf8(), attr_value.utf8()));
}

Vector<Variant> XMLNode::get_children() const {
    Vector<Variant>  array;
    xml_object_range<xml_node_iterator> range = m_pNode->children();
    for(xml_node_iterator it = range.begin(); it != range.end(); ++it )
        array.push_back(create(*it));
    return array;
}

Vector<Variant> XMLNode::get_elements(const String &name) const {
    Vector<Variant>  array;
    xml_object_range<xml_named_node_iterator> range = m_pNode->children(name.utf8());
    for(xml_named_node_iterator it = range.begin(); it != range.end(); ++it )
        array.push_back(create(*it).ptr());
    return array;
}

Vector<Variant> XMLNode::get_attributes() const {
    Vector<Variant>  array;
    xml_object_range<xml_attribute_iterator> range = m_pNode->attributes();
    for(xml_attribute_iterator it = range.begin(); it != range.end(); ++it )
        array.push_back(XMLAttribute::create(*it).ptr());
    return array;
}

void XMLNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("empty"), &XMLNode::empty);
	ClassDB::bind_method(D_METHOD("type"), &XMLNode::type);
	ClassDB::bind_method(D_METHOD("get_name"), &XMLNode::name);
	ClassDB::bind_method(D_METHOD("get_value"), &XMLNode::value);

	ClassDB::bind_method(D_METHOD("first_attribute:XMLAttribute"), &XMLNode::first_attribute);
	ClassDB::bind_method(D_METHOD("last_attribute:XMLAttribute"), &XMLNode::last_attribute);

	ClassDB::bind_method(D_METHOD("first_child:XMLNode"), &XMLNode::first_child);
	ClassDB::bind_method(D_METHOD("last_child:XMLNode"), &XMLNode::last_child);

	ClassDB::bind_method(D_METHOD("next_sibling:XMLNode"), &XMLNode::next_sibling);
	ClassDB::bind_method(D_METHOD("previous_sibling:XMLNode"), &XMLNode::previous_sibling);

	ClassDB::bind_method(D_METHOD("parent:XMLNode"), &XMLNode::parent);
	ClassDB::bind_method(D_METHOD("root:XMLNode"), &XMLNode::root);
	ClassDB::bind_method(D_METHOD("text:XMLText"), &XMLNode::text);

	ClassDB::bind_method(D_METHOD("child:XMLNode", "name"), &XMLNode::child);
	ClassDB::bind_method(D_METHOD("attribute:XMLAttribute", "name"), &XMLNode::attribute);
	ClassDB::bind_method(D_METHOD("next_element:XMLNode", "name"), &XMLNode::next_element);
	ClassDB::bind_method(D_METHOD("previous_element:XMLNode", "name"), &XMLNode::previous_element);

	ClassDB::bind_method(D_METHOD("child_value"), &XMLNode::child_value);
	ClassDB::bind_method(D_METHOD("element_value", "name"), &XMLNode::element_value);

	ClassDB::bind_method(D_METHOD("set_name", "name"), &XMLNode::set_name);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &XMLNode::set_value);

	ClassDB::bind_method(D_METHOD("append_attribute:XMLAttribute", "name"), &XMLNode::append_attribute);
	ClassDB::bind_method(D_METHOD("prepend_attribute:XMLAttribute", "name"), &XMLNode::prepend_attribute);
	ClassDB::bind_method(D_METHOD("insert_attribute_after:XMLAttribute", "name", "attr:XMLAttribute"), &XMLNode::insert_attribute_after);
	ClassDB::bind_method(D_METHOD("insert_attribute_before:XMLAttribute", "name", "attr:XMLAttribute"), &XMLNode::insert_attribute_before);

	ClassDB::bind_method(D_METHOD("append_child:XMLNode", "node_type"), &XMLNode::append_child);
	ClassDB::bind_method(D_METHOD("prepend_child:XMLNode", "node_type"), &XMLNode::prepend_child);
	ClassDB::bind_method(D_METHOD("insert_child_after:XMLNode", "node_type", "node:XMLNode"), &XMLNode::insert_child_after);
	ClassDB::bind_method(D_METHOD("insert_child_before:XMLNode", "node_type", "node:XMLNode"), &XMLNode::insert_child_before);

	ClassDB::bind_method(D_METHOD("append_element:XMLNode", "name"), &XMLNode::append_element);
	ClassDB::bind_method(D_METHOD("prepend_element:XMLNode", "name"), &XMLNode::prepend_element);
	ClassDB::bind_method(D_METHOD("insert_element_after:XMLNode", "name", "node:XMLNode"), &XMLNode::insert_element_after);
	ClassDB::bind_method(D_METHOD("insert_element_before:XMLNode", "name", "node:XMLNode"), &XMLNode::insert_element_before);

	ClassDB::bind_method(D_METHOD("append_copy:XMLNode", "proto:XMLNode"), &XMLNode::append_copy);
	ClassDB::bind_method(D_METHOD("prepend_element:XMLNode", "proto:XMLNode"), &XMLNode::prepend_copy);
	ClassDB::bind_method(D_METHOD("insert_copy_after:XMLNode", "proto:XMLNode", "node:XMLNode"), &XMLNode::insert_copy_after);
	ClassDB::bind_method(D_METHOD("insert_copy_before:XMLNode", "proto:XMLNode", "node:XMLNode"), &XMLNode::insert_copy_before);

	ClassDB::bind_method(D_METHOD("append_move:XMLNode", "proto:XMLNode"), &XMLNode::append_move);
	ClassDB::bind_method(D_METHOD("prepend_move:XMLNode", "proto:XMLNode"), &XMLNode::prepend_move);
	ClassDB::bind_method(D_METHOD("insert_move_after:XMLNode", "proto:XMLNode", "node:XMLNode"), &XMLNode::insert_move_after);
	ClassDB::bind_method(D_METHOD("insert_move_before:XMLNode", "proto:XMLNode", "node:XMLNode"), &XMLNode::insert_move_before);

	ClassDB::bind_method(D_METHOD("remove_attribute", "attr:XMLAttribute"), &XMLNode::remove_attribute);
	ClassDB::bind_method(D_METHOD("remove_attribute_by_name", "name"), &XMLNode::remove_attribute_by_name);

	ClassDB::bind_method(D_METHOD("remove_child", "attr:XMLNode"), &XMLNode::remove_child);
	ClassDB::bind_method(D_METHOD("remove_child_by_name", "name"), &XMLNode::remove_child_by_name);

	ClassDB::bind_method(D_METHOD("first_element_by_path:XMLNode", "path"), &XMLNode::first_element_by_path);
	ClassDB::bind_method(D_METHOD("find_child_by_attribute:XMLNode", "attr_name", "attr_value", "ele_name"), &XMLNode::find_child_by_attribute);
	ClassDB::bind_method(D_METHOD("get_children"), &XMLNode::get_children);
	ClassDB::bind_method(D_METHOD("get_elements", "name"), &XMLNode::get_elements);
	ClassDB::bind_method(D_METHOD("get_attributes"), &XMLNode::get_attributes);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "get_name", "set_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "get_value", "set_value");

	BIND_CONSTANT(NODE_NULL);
	BIND_CONSTANT(NODE_DOCUMENT);
	BIND_CONSTANT(NODE_ELEMENT);
	BIND_CONSTANT(NODE_PCDATA);
	BIND_CONSTANT(NODE_CDDATA);
	BIND_CONSTANT(NODE_COMMENT);
	BIND_CONSTANT(NODE_PI);
	BIND_CONSTANT(NODE_DECLARATION);
	BIND_CONSTANT(NODE_DOCTYPE);
}

void XMLDocument::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_string", "xml_content"), &XMLDocument::load_string);
	ClassDB::bind_method(D_METHOD("save_string"), &XMLDocument::save_string, DEFVAL("\t"));
	ClassDB::bind_method(D_METHOD("load_file", "path"), &XMLDocument::load_file);
	ClassDB::bind_method(D_METHOD("save_file"), &XMLDocument::save_file, DEFVAL("\t"));
}

XMLDocument::XMLDocument(): XMLNode() {
	delete m_pNode;
	m_pNode = new xml_document;
//	print_line("Contructing XMLDocument");
}

XMLDocument::~XMLDocument() {
	if (m_pNode)
		delete static_cast<xml_document*>(m_pNode);
	m_pNode = NULL;
//	print_line("Destructing XMLDocument");
}

Error XMLDocument::load_string(const String &contents) {
	if (static_cast<xml_document*>(m_pNode)->load_string(contents.utf8()))
		return OK;
	else
		return FAILED;
}

Error XMLDocument::load_file(const String &path) {
	String gpath = GlobalConfig::get_singleton()->globalize_path(path);
	Error err = OK;
	FileAccess *file = file->open(gpath, FileAccess::READ, &err);
	if (err == OK) {
		String content;
		while (!file->eof_reached()) {
			content += file->get_line() + "\n";
		}
		if (static_cast<xml_document*>(m_pNode)->load_string(content.utf8(), encoding_utf8))
			err = OK;
		else
			err = FAILED;
	}
	if (file)
		memdelete(file);
	return err;
}

String XMLDocument::save_string(const String &indent) {
	std::stringstream ss;
	static_cast<pugi::xml_document*>(m_pNode)->save(ss, indent.utf8(), format_default, encoding_utf8);
	String s;
	s.parse_utf8(ss.str().c_str());
	return s;
}

Error XMLDocument::save_file(const String &path, const String &indent) {
	std::stringstream ss;
	Error err = FAILED;
	if (FileAccess *file = FileAccess::open(GlobalConfig::get_singleton()->globalize_path(path), FileAccess::WRITE)) {
		if (file->get_error() == OK) {
			String s = save_string(indent);
			file->store_string(s);
			file->close();
		}
		err = file->get_error();
		memdelete(file);
	}
	return err;
}

void XMLAttribute::_bind_methods() {
	ClassDB::bind_method(D_METHOD("empty"), &XMLAttribute::empty);

	ClassDB::bind_method(D_METHOD("get_name"), &XMLAttribute::name);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &XMLAttribute::set_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "get_name", "set_name");

	ClassDB::bind_method(D_METHOD("get_value"), &XMLAttribute::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &XMLAttribute::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "get_value", "set_value");

}



XMLAttribute::XMLAttribute(){
	m_pAttr = new xml_attribute;
}

XMLAttribute::~XMLAttribute() {
	if(m_pAttr)
		delete m_pAttr;
	m_pAttr = NULL;
}

Ref<XMLAttribute> XMLAttribute::create(const xml_attribute &native_attr) {
	XMLAttribute *attr = memnew(XMLAttribute);
	attr->_set_native_attr(native_attr);
	return attr;
}

void XMLAttribute::_set_native_attr(const xml_attribute &native_attr) {
	if(m_pAttr)
		delete m_pAttr;
	m_pAttr = new xml_attribute(native_attr);
}


XMLText::XMLText(){
	m_pText = new xml_text;
}

XMLText::~XMLText() {
	if(m_pText)
		delete m_pText;
	m_pText = NULL;
}

Ref<XMLText> XMLText::create(const xml_text &native_text) {
	XMLText * text = memnew(XMLText);
	text->_set_native_text(native_text);
	return text;
}

Ref<XMLNode> XMLText::data() {
	return XMLNode::create(m_pText->data());
}

void XMLText::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_value"), &XMLText::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "text"), &XMLText::set_value);
	ClassDB::bind_method(D_METHOD("data:XMLNode", "data"), &XMLText::data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "get_value", "set_value");
}

void XMLText::_set_native_text(const xml_text &native_text) {
	if(m_pText)
		delete m_pText;
	m_pText = new xml_text(native_text);
}


String str_p2g(const char *str) {
	String s;
	s.parse_utf8(str);
	return s;
}
