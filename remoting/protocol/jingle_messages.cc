// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "remoting/protocol/jingle_messages.h"

#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "remoting/base/constants.h"
#include "remoting/protocol/content_description.h"
#include "remoting/protocol/name_value_map.h"
#include "third_party/webrtc/libjingle/xmllite/xmlelement.h"

using buzz::QName;
using buzz::XmlElement;

namespace remoting {
namespace protocol {

namespace {

const char kJabberNamespace[] = "jabber:client";
const char kJingleNamespace[] = "urn:xmpp:jingle:1";

// Namespace for transport messages when using standard ICE.
const char kIceTransportNamespace[] = "google:remoting:ice";

const char kWebrtcTransportNamespace[] = "google:remoting:webrtc";

const char kEmptyNamespace[] = "";
const char kXmlNamespace[] = "http://www.w3.org/XML/1998/namespace";

const int kPortMin = 1000;
const int kPortMax = 65535;

const NameMapElement<SignalingAddress::Channel> kChannelTypes[] = {
    {SignalingAddress::Channel::LCS, "lcs"},
    {SignalingAddress::Channel::XMPP, "xmpp"},
};

const NameMapElement<JingleMessage::ActionType> kActionTypes[] = {
  { JingleMessage::SESSION_INITIATE, "session-initiate" },
  { JingleMessage::SESSION_ACCEPT, "session-accept" },
  { JingleMessage::SESSION_TERMINATE, "session-terminate" },
  { JingleMessage::SESSION_INFO, "session-info" },
  { JingleMessage::TRANSPORT_INFO, "transport-info" },
};

const NameMapElement<JingleMessage::Reason> kReasons[] = {
  { JingleMessage::SUCCESS, "success" },
  { JingleMessage::DECLINE, "decline" },
  { JingleMessage::CANCEL, "cancel" },
  { JingleMessage::EXPIRED, "expired" },
  { JingleMessage::GENERAL_ERROR, "general-error" },
  { JingleMessage::FAILED_APPLICATION, "failed-application" },
  { JingleMessage::INCOMPATIBLE_PARAMETERS, "incompatible-parameters" },
};

bool ParseIceCredentials(const buzz::XmlElement* element,
                         IceTransportInfo::IceCredentials* credentials) {
  DCHECK(element->Name() == QName(kIceTransportNamespace, "credentials"));

  const std::string& channel = element->Attr(QName(kEmptyNamespace, "channel"));
  const std::string& ufrag =
      element->Attr(QName(kEmptyNamespace, "ufrag"));
  const std::string& password =
      element->Attr(QName(kEmptyNamespace, "password"));

  if (channel.empty() || ufrag.empty() || password.empty()) {
    return false;
  }

  credentials->channel = channel;
  credentials->ufrag = ufrag;
  credentials->password = password;

  return true;
}

bool ParseIceCandidate(const buzz::XmlElement* element,
                       IceTransportInfo::NamedCandidate* candidate) {
  DCHECK(element->Name() == QName(kIceTransportNamespace, "candidate"));

  const std::string& name = element->Attr(QName(kEmptyNamespace, "name"));
  const std::string& foundation =
      element->Attr(QName(kEmptyNamespace, "foundation"));
  const std::string& address = element->Attr(QName(kEmptyNamespace, "address"));
  const std::string& port_str = element->Attr(QName(kEmptyNamespace, "port"));
  const std::string& type = element->Attr(QName(kEmptyNamespace, "type"));
  const std::string& protocol =
      element->Attr(QName(kEmptyNamespace, "protocol"));
  const std::string& priority_str =
      element->Attr(QName(kEmptyNamespace, "priority"));
  const std::string& generation_str =
      element->Attr(QName(kEmptyNamespace, "generation"));

  int port;
  unsigned priority;
  int generation;
  if (name.empty() || foundation.empty() || address.empty() ||
      !base::StringToInt(port_str, &port) || port < kPortMin ||
      port > kPortMax || type.empty() || protocol.empty() ||
      !base::StringToUint(priority_str, &priority) ||
      !base::StringToInt(generation_str, &generation)) {
    return false;
  }

  candidate->name = name;

  candidate->candidate.set_foundation(foundation);
  candidate->candidate.set_address(rtc::SocketAddress(address, port));
  candidate->candidate.set_type(type);
  candidate->candidate.set_protocol(protocol);
  candidate->candidate.set_priority(priority);
  candidate->candidate.set_generation(generation);

  return true;
}

XmlElement* FormatIceCredentials(
    const IceTransportInfo::IceCredentials& credentials) {
  XmlElement* result =
      new XmlElement(QName(kIceTransportNamespace, "credentials"));
  result->SetAttr(QName(kEmptyNamespace, "channel"), credentials.channel);
  result->SetAttr(QName(kEmptyNamespace, "ufrag"), credentials.ufrag);
  result->SetAttr(QName(kEmptyNamespace, "password"), credentials.password);
  return result;
}

XmlElement* FormatIceCandidate(
    const IceTransportInfo::NamedCandidate& candidate) {
  XmlElement* result =
      new XmlElement(QName(kIceTransportNamespace, "candidate"));
  result->SetAttr(QName(kEmptyNamespace, "name"), candidate.name);
  result->SetAttr(QName(kEmptyNamespace, "foundation"),
                  candidate.candidate.foundation());
  result->SetAttr(QName(kEmptyNamespace, "address"),
                  candidate.candidate.address().ipaddr().ToString());
  result->SetAttr(QName(kEmptyNamespace, "port"),
                  base::UintToString(candidate.candidate.address().port()));
  result->SetAttr(QName(kEmptyNamespace, "type"), candidate.candidate.type());
  result->SetAttr(QName(kEmptyNamespace, "protocol"),
                  candidate.candidate.protocol());
  result->SetAttr(QName(kEmptyNamespace, "priority"),
                  base::UintToString(candidate.candidate.priority()));
  result->SetAttr(QName(kEmptyNamespace, "generation"),
                  base::UintToString(candidate.candidate.generation()));
  return result;
}

// Represents the XML attrbute names for the various address fields in the
// iq stanza.
enum class Field { JID, CHANNEL, ENDPOINT_ID };

buzz::QName GetQNameByField(Field attr, bool from) {
  std::string attribute_name;
  switch (attr) {
    case Field::JID:
      attribute_name = (from) ? "from" : "to";
      break;
    case Field::ENDPOINT_ID:
      attribute_name = (from) ? "from-endpoint-id" : "to-endpoint-id";
      break;
    case Field::CHANNEL:
      attribute_name = (from) ? "from-channel" : "to-channel";
      break;
    default:
      NOTREACHED();
  }
  return QName(kEmptyNamespace, attribute_name);
}

SignalingAddress ParseAddress(
    const buzz::XmlElement* iq, bool from, std::string* error) {
  SignalingAddress empty_instance;

  std::string jid(iq->Attr(GetQNameByField(Field::JID, from)));

  const XmlElement* jingle = iq->FirstNamed(QName(kJingleNamespace, "jingle"));

  if (!jingle) {
    return SignalingAddress(jid);
  }

  std::string type(iq->Attr(QName(std::string(), "type")));
  // For error IQs, flips the |from| flag as the jingle node represents the
  // original request.
  if (type == "error") {
    from = !from;
  }

  std::string endpoint_id(
      jingle->Attr(GetQNameByField(Field::ENDPOINT_ID, from)));
  std::string channel_str(jingle->Attr(GetQNameByField(Field::CHANNEL, from)));
  SignalingAddress::Channel channel;

  if (channel_str.empty()) {
    channel = SignalingAddress::Channel::XMPP;
  } else if (!NameToValue(kChannelTypes, channel_str, &channel)) {
    *error = "Unknown channel: " + channel_str;
    return empty_instance;
  }

  bool isLcs = (channel == SignalingAddress::Channel::LCS);

  if (isLcs == endpoint_id.empty()) {
    *error = (isLcs ? "Missing |endpoint-id| for LCS channel"
                    : "|endpoint_id| should be empty for XMPP channel");
    return empty_instance;
  }

  return SignalingAddress(jid, endpoint_id, channel);
}

void SetAddress(buzz::XmlElement* iq,
                buzz::XmlElement* jingle,
                const SignalingAddress& address,
                bool from) {
  if (address.empty()) {
    return;
  }

  // Always set the JID.
  iq->SetAttr(GetQNameByField(Field::JID, from), address.jid);

  // Do not tamper the routing-info in the jingle tag for error IQ's, as
  // it corresponds to the original message.
  std::string type(iq->Attr(QName(std::string(), "type")));
  if (type == "error") {
    return;
  }

  // Start from a fresh slate regardless of the previous address format.
  jingle->ClearAttr(GetQNameByField(Field::CHANNEL, from));
  jingle->ClearAttr(GetQNameByField(Field::ENDPOINT_ID, from));

  // Only set the channel and endpoint_id in the LCS channel.
  if (address.channel == SignalingAddress::Channel::LCS) {
    jingle->AddAttr(
        GetQNameByField(Field::ENDPOINT_ID, from), address.endpoint_id);
    jingle->AddAttr(
        GetQNameByField(Field::CHANNEL, from),
        ValueToName(kChannelTypes, address.channel));
  }
}

}  // namespace

IceTransportInfo::NamedCandidate::NamedCandidate(
    const std::string& name,
    const cricket::Candidate& candidate)
    : name(name),
      candidate(candidate) {
}

IceTransportInfo::IceCredentials::IceCredentials(std::string channel,
                                              std::string ufrag,
                                              std::string password)
    : channel(channel), ufrag(ufrag), password(password) {
}

SignalingAddress::SignalingAddress()
    : channel(SignalingAddress::Channel::XMPP) {}

SignalingAddress::SignalingAddress(const std::string& jid)
    : jid(jid), channel(SignalingAddress::Channel::XMPP) {}

SignalingAddress::SignalingAddress(const std::string& jid,
                                   const std::string& endpoint_id,
                                   Channel channel)
    : jid(jid), endpoint_id(endpoint_id), channel(channel) {}

bool SignalingAddress::operator==(const SignalingAddress& other) {
  return (other.endpoint_id == endpoint_id) && (other.jid == jid) &&
         (other.channel == channel);
}

bool SignalingAddress::operator!=(const SignalingAddress& other) {
  return !(*this == other);
}

// static
bool JingleMessage::IsJingleMessage(const buzz::XmlElement* stanza) {
  return stanza->Name() == QName(kJabberNamespace, "iq") &&
         stanza->Attr(QName(std::string(), "type")) == "set" &&
         stanza->FirstNamed(QName(kJingleNamespace, "jingle")) != nullptr;
}

// static
std::string JingleMessage::GetActionName(ActionType action) {
  return ValueToName(kActionTypes, action);
}

JingleMessage::JingleMessage() {}

JingleMessage::JingleMessage(const SignalingAddress& to,
                             ActionType action,
                             const std::string& sid)
    : to(to), action(action), sid(sid) {}

JingleMessage::~JingleMessage() {}

bool JingleMessage::ParseXml(const buzz::XmlElement* stanza,
                             std::string* error) {
  if (!IsJingleMessage(stanza)) {
    *error = "Not a jingle message";
    return false;
  }

  const XmlElement* jingle_tag =
      stanza->FirstNamed(QName(kJingleNamespace, "jingle"));
  if (!jingle_tag) {
    *error = "Not a jingle message";
    return false;
  }

  from = ParseAddress(stanza, true, error);
  if (!error->empty()) {
    return false;
  }

  to = ParseAddress(stanza, false, error);
  if (!error->empty()) {
    return false;
  }

  initiator = jingle_tag->Attr(QName(kEmptyNamespace, "initiator"));

  std::string action_str = jingle_tag->Attr(QName(kEmptyNamespace, "action"));
  if (action_str.empty()) {
    *error = "action attribute is missing";
    return false;
  }
  if (!NameToValue(kActionTypes, action_str, &action)) {
    *error = "Unknown action " + action_str;
    return false;
  }

  sid = jingle_tag->Attr(QName(kEmptyNamespace, "sid"));
  if (sid.empty()) {
    *error = "sid attribute is missing";
    return false;
  }

  if (action == SESSION_INFO) {
    // session-info messages may contain arbitrary information not
    // defined by the Jingle protocol. We don't need to parse it.
    const XmlElement* child = jingle_tag->FirstElement();
    if (child) {
      // session-info is allowed to be empty.
      info.reset(new XmlElement(*child));
    } else {
      info.reset(nullptr);
    }
    return true;
  }

  const XmlElement* reason_tag =
      jingle_tag->FirstNamed(QName(kJingleNamespace, "reason"));
  if (reason_tag && reason_tag->FirstElement()) {
    if (!NameToValue(kReasons, reason_tag->FirstElement()->Name().LocalPart(),
                     &reason)) {
      reason = UNKNOWN_REASON;
    }
  }

  const XmlElement* error_code_tag =
      jingle_tag->FirstNamed(QName(kChromotingXmlNamespace, "error-code"));
  if (error_code_tag && !error_code_tag->BodyText().empty()) {
    if (!ParseErrorCode(error_code_tag->BodyText(), &error_code)) {
      LOG(WARNING) << "Unknown error-code received "
                   << error_code_tag->BodyText();
      error_code = UNKNOWN_ERROR;
    }
  }

  if (action == SESSION_TERMINATE)
    return true;

  const XmlElement* content_tag =
      jingle_tag->FirstNamed(QName(kJingleNamespace, "content"));
  if (!content_tag) {
    *error = "content tag is missing";
    return false;
  }

  std::string content_name = content_tag->Attr(QName(kEmptyNamespace, "name"));
  if (content_name != ContentDescription::kChromotingContentName) {
    *error = "Unexpected content name: " + content_name;
    return false;
  }

  const XmlElement* webrtc_transport_tag = content_tag->FirstNamed(
      QName(kWebrtcTransportNamespace, "transport"));
  if (webrtc_transport_tag) {
    transport_info.reset(new buzz::XmlElement(*webrtc_transport_tag));
  }

  description.reset(nullptr);
  if (action == SESSION_INITIATE || action == SESSION_ACCEPT) {
    const XmlElement* description_tag = content_tag->FirstNamed(
        QName(kChromotingXmlNamespace, "description"));
    if (!description_tag) {
      *error = "Missing chromoting content description";
      return false;
    }

    description = ContentDescription::ParseXml(description_tag,
                                               webrtc_transport_tag != nullptr);
    if (!description.get()) {
      *error = "Failed to parse content description";
      return false;
    }
  }

  if (!webrtc_transport_tag) {
    const XmlElement* ice_transport_tag = content_tag->FirstNamed(
        QName(kIceTransportNamespace, "transport"));
    if (ice_transport_tag) {
      transport_info.reset(new buzz::XmlElement(*ice_transport_tag));
    }
  }

  return true;
}

std::unique_ptr<buzz::XmlElement> JingleMessage::ToXml() const {
  std::unique_ptr<XmlElement> root(
      new XmlElement(QName("jabber:client", "iq"), true));

  DCHECK(!to.empty());
  root->SetAttr(QName(kEmptyNamespace, "type"), "set");

  XmlElement* jingle_tag =
      new XmlElement(QName(kJingleNamespace, "jingle"), true);
  root->AddElement(jingle_tag);
  jingle_tag->AddAttr(QName(kEmptyNamespace, "sid"), sid);
  SetAddress(root.get(), jingle_tag, to, false);
  SetAddress(root.get(), jingle_tag, from, true);

  const char* action_attr = ValueToName(kActionTypes, action);
  if (!action_attr)
    LOG(FATAL) << "Invalid action value " << action;
  jingle_tag->AddAttr(QName(kEmptyNamespace, "action"), action_attr);

  if (action == SESSION_INFO) {
    if (info.get())
      jingle_tag->AddElement(new XmlElement(*info.get()));
    return root;
  }

  if (action == SESSION_INITIATE)
    jingle_tag->AddAttr(QName(kEmptyNamespace, "initiator"), initiator);

  if (reason != UNKNOWN_REASON) {
    XmlElement* reason_tag = new XmlElement(QName(kJingleNamespace, "reason"));
    jingle_tag->AddElement(reason_tag);
    reason_tag->AddElement(new XmlElement(
        QName(kJingleNamespace, ValueToName(kReasons, reason))));

    if (error_code != UNKNOWN_ERROR) {
      XmlElement* error_code_tag =
          new XmlElement(QName(kChromotingXmlNamespace, "error-code"));
      jingle_tag->AddElement(error_code_tag);
      error_code_tag->SetBodyText(ErrorCodeToString(error_code));
    }
  }

  if (action != SESSION_TERMINATE) {
    XmlElement* content_tag =
        new XmlElement(QName(kJingleNamespace, "content"));
    jingle_tag->AddElement(content_tag);

    content_tag->AddAttr(QName(kEmptyNamespace, "name"),
                         ContentDescription::kChromotingContentName);
    content_tag->AddAttr(QName(kEmptyNamespace, "creator"), "initiator");

    if (description)
      content_tag->AddElement(description->ToXml());

    if (transport_info) {
      content_tag->AddElement(new XmlElement(*transport_info));
    } else if (description && description->config()->webrtc_supported()) {
      content_tag->AddElement(
          new XmlElement(QName(kWebrtcTransportNamespace, "transport")));
    }
  }

  return root;
}

JingleMessageReply::JingleMessageReply()
    : type(REPLY_RESULT),
      error_type(NONE) {
}

JingleMessageReply::JingleMessageReply(ErrorType error)
    : type(error != NONE ? REPLY_ERROR : REPLY_RESULT),
      error_type(error) {
}

JingleMessageReply::JingleMessageReply(ErrorType error,
                                       const std::string& text_value)
    : type(REPLY_ERROR),
      error_type(error),
      text(text_value) {
}

JingleMessageReply::~JingleMessageReply() { }

std::unique_ptr<buzz::XmlElement> JingleMessageReply::ToXml(
    const buzz::XmlElement* request_stanza) const {
  std::unique_ptr<XmlElement> iq(
      new XmlElement(QName(kJabberNamespace, "iq"), true));

  iq->SetAttr(QName(kEmptyNamespace, "id"),
              request_stanza->Attr(QName(kEmptyNamespace, "id")));

  SignalingAddress original_from;
  std::string error_message;
  original_from = ParseAddress(request_stanza, true, &error_message);
  DCHECK(error_message.empty());

  if (type == REPLY_RESULT) {
    iq->SetAttr(QName(kEmptyNamespace, "type"), "result");
    XmlElement* jingle =
        new XmlElement(QName(kJingleNamespace, "jingle"), true);
    iq->AddElement(jingle);
    SetAddress(iq.get(), jingle, original_from, false);
    return iq;
  }

  DCHECK_EQ(type, REPLY_ERROR);

  iq->SetAttr(QName(kEmptyNamespace, "type"), "error");
  SetAddress(iq.get(), nullptr, original_from, false);

  for (const buzz::XmlElement* child = request_stanza->FirstElement();
       child != nullptr; child = child->NextElement()) {
    iq->AddElement(new buzz::XmlElement(*child));
  }

  buzz::XmlElement* error =
      new buzz::XmlElement(QName(kJabberNamespace, "error"));
  iq->AddElement(error);

  std::string type;
  std::string error_text;
  QName name;
  switch (error_type) {
    case BAD_REQUEST:
      type = "modify";
      name = QName(kJabberNamespace, "bad-request");
      break;
    case NOT_IMPLEMENTED:
      type = "cancel";
      name = QName(kJabberNamespace, "feature-bad-request");
      break;
    case INVALID_SID:
      type = "modify";
      name = QName(kJabberNamespace, "item-not-found");
      error_text = "Invalid SID";
      break;
    case UNEXPECTED_REQUEST:
      type = "modify";
      name = QName(kJabberNamespace, "unexpected-request");
      break;
    case UNSUPPORTED_INFO:
      type = "modify";
      name = QName(kJabberNamespace, "feature-not-implemented");
      break;
    default:
      NOTREACHED();
  }

  if (!text.empty())
    error_text = text;

  error->SetAttr(QName(kEmptyNamespace, "type"), type);

  // If the error name is not in the standard namespace, we have
  // to first add some error from that namespace.
  if (name.Namespace() != kJabberNamespace) {
    error->AddElement(
        new buzz::XmlElement(QName(kJabberNamespace, "undefined-condition")));
  }
  error->AddElement(new buzz::XmlElement(name));

  if (!error_text.empty()) {
    // It's okay to always use English here. This text is for
    // debugging purposes only.
    buzz::XmlElement* text_elem =
            new buzz::XmlElement(QName(kJabberNamespace, "text"));
    text_elem->SetAttr(QName(kXmlNamespace, "lang"), "en");
    text_elem->SetBodyText(error_text);
    error->AddElement(text_elem);
  }

  return iq;
}

IceTransportInfo::IceTransportInfo() {}
IceTransportInfo::~IceTransportInfo() {}

bool IceTransportInfo::ParseXml(
    const buzz::XmlElement* element) {
  if (element->Name() != QName(kIceTransportNamespace, "transport"))
    return false;

  ice_credentials.clear();
  candidates.clear();

  QName qn_credentials(kIceTransportNamespace, "credentials");
  for (const XmlElement* credentials_tag = element->FirstNamed(qn_credentials);
       credentials_tag;
       credentials_tag = credentials_tag->NextNamed(qn_credentials)) {
    IceTransportInfo::IceCredentials credentials;
    if (!ParseIceCredentials(credentials_tag, &credentials))
      return false;
    ice_credentials.push_back(credentials);
  }

  QName qn_candidate(kIceTransportNamespace, "candidate");
  for (const XmlElement* candidate_tag = element->FirstNamed(qn_candidate);
       candidate_tag; candidate_tag = candidate_tag->NextNamed(qn_candidate)) {
    IceTransportInfo::NamedCandidate candidate;
    if (!ParseIceCandidate(candidate_tag, &candidate))
      return false;
    candidates.push_back(candidate);
  }

  return true;
}

std::unique_ptr<buzz::XmlElement> IceTransportInfo::ToXml() const {
  std::unique_ptr<buzz::XmlElement> result(
      new XmlElement(QName(kIceTransportNamespace, "transport"), true));
  for (const IceCredentials& credentials : ice_credentials) {
    result->AddElement(FormatIceCredentials(credentials));
  }
  for (const NamedCandidate& candidate : candidates) {
    result->AddElement(FormatIceCandidate(candidate));
  }
  return result;
}

}  // namespace protocol
}  // namespace remoting
