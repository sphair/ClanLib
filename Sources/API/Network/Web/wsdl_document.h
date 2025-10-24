
#ifndef header_wsdl_document
#define header_wsdl_document

#include <vector>

class WSDLTypes;
class WSDLSchema;
class WSDLMessage;
class WSDLPart;
class WSDLPortType;
class WSDLBinding;
class WSDLBindingOperation;
class WSDLPort;
class WSDLService;
class WSDLOperation;

/** !hide!*/
class WSDLDocument
{
public:
	CL_DomDocument get_document();

	WSDLTypes get_types();

	std::vector<WSDLMessage> get_messages();

	std::vector<WSDLPortType> get_port_types();

	std::vector<WSDLOperation> get_operations();

	std::vector<WSDLBinding> get_bindings();

	std::vector<WSDLService> get_services();
};

/** !hide!*/
class WSDLTypes
{
public:
	std::vector<WSDLSchema> get_schemas();
};

/** !hide!*/
class WSDLSchema
{
public:
	CL_DomString get_target_namespace() const;
	CL_DomElement get_element();
};

/** !hide!*/
class WSDLMessage
{
public:
	CL_DomString get_name() const;
	std::vector<WSDLPart> get_parts() const;
};

/** !hide!*/
class WSDLPart
{
public:
	CL_DomString get_name() const;
	CL_DomString get_type() const;
};

/** !hide!*/
class WSDLPortType
{
public:
	CL_DomString get_name() const;
	std::vector<WSDLOperation> get_operations() const;
};

/** !hide!*/
class WSDLOperation
{
public:
	CL_DomString get_name() const;
	std::vector<CL_DomString> get_parameter_order() const;

	CL_DomString get_input_name();
	CL_DomString get_input_message();

	CL_DomString get_output_name();
	CL_DomString get_output_message();

	CL_DomString get_fault_name();
	CL_DomString get_fault_message();
};

/** !hide!*/
class WSDLBinding
{
public:
	CL_DomString get_name() const;
	CL_DomString get_soap_style() const;
	CL_DomString get_soap_transport() const;
	std::vector<WSDLBindingOperation> get_operations() const;
};

/** !hide!*/
class WSDLBindingOperation
{
public:
	CL_DomString get_soap_action() const;

	CL_DomString get_input_name() const;
	CL_DomString get_input_soap_encoding_style() const;
	CL_DomString get_input_namespace() const;
	CL_DomString get_input_use() const;

	CL_DomString get_output_name() const;
	CL_DomString get_output_soap_encoding_style() const;
	CL_DomString get_output_namespace() const;
	CL_DomString get_output_use() const;

	CL_DomString get_fault_name() const;
	CL_DomString get_fault_soap_encoding_style() const;
	CL_DomString get_fault_namespace() const;
	CL_DomString get_fault_use() const;
};

/** !hide!*/
class WSDLService
{
public:
	CL_DomString get_name() const;
	std::vector<WSDLPort> get_ports() const;
};

/** !hide!*/
class WSDLPort
{
public:
	CL_DomString get_name() const;
	CL_DomString get_binding() const;
	CL_DomString get_soap_address_location() const;
};

/*
definitions
  targetNamespace="http:/blablabla"
  xmlns="http:/** schemas.xmlsoap.org/wsdl/"*/
  xmlns:namespace="http:/blablabla"
  xmlns:wsdlsoap="http:/** schemas.xmlsoap.org/wsdl/soap/"
  xmlns:xsd="http:    www.w3.org/2001/XMLSchema"*/

  types
    schema targetNamespace="http:/blablabla-standard-types" xmlns="http:/** www.w3.org/2001/XMLSchema"*/
      <--XMLSchema here-->
    schema targetNamespace="http:/blablabla" xmlns="http:/** www.w3.org/2001/XMLSchema"*/
      <--XMLSchema here-->

  message name="FuncNameRequest"
    part name="ParamName" type="xsd:string"

  portType name="PortName"
    operation name="FuncName" parameterOrder="params list"
      input message="namespace:FuncNameRequest" name="FuncNameRequest"
      output message="namespace:FuncNameResposne" name="FuncNameResponse"
      fault message="namespace:SomeException" name="SomeException"
      fault message="namespace:SomeOtherException" name="SomeOtherException"

  binding name="Binding" type="namespace:PortName"
    wsdlsoap:binding style="rpc" transport="http:/** schemas.xmlsoap.org/soap/http"*/
    operation name="FuncName"
      wsdlsoap:operation soapAction=""
      input name="FuncNameRequest"
	wsdlsoap:body encodingStyle="http:/** schemas.xmlsoap.org/soap/encoding/" namespace="http:/blablabla" use="encoded"*/
      output name="FuncNameResponse"
	wsdlsoap:body encodingStyle="http:/** schemas.xmlsoap.org/soap/encoding/" namespace="http:/blablabla" use="encoded"*/
      fault name="SomeException"
	wsdlsoap:body encodingStyle="http:/** schemas.xmlsoap.org/soap/encoding/" namespace="http:/blablabla" use="encoded"*/
      fault name="SomeOtherException"
	wsdlsoap:body encodingStyle="http:/** schemas.xmlsoap.org/soap/encoding/" namespace="http:/blablabla" use="encoded"*/

  service name="TestWebService"
    port binding="namespace:Binding" name="PortName"
      wsdlsoap:address location="http:/** test.blablabla.dk/services/webservice"*/
*/
#endif
