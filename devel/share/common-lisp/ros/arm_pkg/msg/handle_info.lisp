; Auto-generated. Do not edit!


(cl:in-package arm_pkg-msg)


;//! \htmlinclude handle_info.msg.html

(cl:defclass <handle_info> (roslisp-msg-protocol:ros-message)
  ((left_x
    :reader left_x
    :initarg :left_x
    :type cl:float
    :initform 0.0)
   (left_y
    :reader left_y
    :initarg :left_y
    :type cl:float
    :initform 0.0)
   (right_x
    :reader right_x
    :initarg :right_x
    :type cl:float
    :initform 0.0)
   (right_y
    :reader right_y
    :initarg :right_y
    :type cl:float
    :initform 0.0)
   (left_putter_a
    :reader left_putter_a
    :initarg :left_putter_a
    :type cl:fixnum
    :initform 0)
   (right_putter_b
    :reader right_putter_b
    :initarg :right_putter_b
    :type cl:fixnum
    :initform 0)
   (left_button_vra
    :reader left_button_vra
    :initarg :left_button_vra
    :type cl:float
    :initform 0.0)
   (right_button_vrb
    :reader right_button_vrb
    :initarg :right_button_vrb
    :type cl:float
    :initform 0.0)
   (connected
    :reader connected
    :initarg :connected
    :type cl:fixnum
    :initform 0))
)

(cl:defclass handle_info (<handle_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <handle_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'handle_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name arm_pkg-msg:<handle_info> is deprecated: use arm_pkg-msg:handle_info instead.")))

(cl:ensure-generic-function 'left_x-val :lambda-list '(m))
(cl:defmethod left_x-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:left_x-val is deprecated.  Use arm_pkg-msg:left_x instead.")
  (left_x m))

(cl:ensure-generic-function 'left_y-val :lambda-list '(m))
(cl:defmethod left_y-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:left_y-val is deprecated.  Use arm_pkg-msg:left_y instead.")
  (left_y m))

(cl:ensure-generic-function 'right_x-val :lambda-list '(m))
(cl:defmethod right_x-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:right_x-val is deprecated.  Use arm_pkg-msg:right_x instead.")
  (right_x m))

(cl:ensure-generic-function 'right_y-val :lambda-list '(m))
(cl:defmethod right_y-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:right_y-val is deprecated.  Use arm_pkg-msg:right_y instead.")
  (right_y m))

(cl:ensure-generic-function 'left_putter_a-val :lambda-list '(m))
(cl:defmethod left_putter_a-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:left_putter_a-val is deprecated.  Use arm_pkg-msg:left_putter_a instead.")
  (left_putter_a m))

(cl:ensure-generic-function 'right_putter_b-val :lambda-list '(m))
(cl:defmethod right_putter_b-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:right_putter_b-val is deprecated.  Use arm_pkg-msg:right_putter_b instead.")
  (right_putter_b m))

(cl:ensure-generic-function 'left_button_vra-val :lambda-list '(m))
(cl:defmethod left_button_vra-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:left_button_vra-val is deprecated.  Use arm_pkg-msg:left_button_vra instead.")
  (left_button_vra m))

(cl:ensure-generic-function 'right_button_vrb-val :lambda-list '(m))
(cl:defmethod right_button_vrb-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:right_button_vrb-val is deprecated.  Use arm_pkg-msg:right_button_vrb instead.")
  (right_button_vrb m))

(cl:ensure-generic-function 'connected-val :lambda-list '(m))
(cl:defmethod connected-val ((m <handle_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader arm_pkg-msg:connected-val is deprecated.  Use arm_pkg-msg:connected instead.")
  (connected m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <handle_info>) ostream)
  "Serializes a message object of type '<handle_info>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'left_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'left_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'right_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'right_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'left_putter_a)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'right_putter_b)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'left_button_vra))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'right_button_vrb))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'connected)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <handle_info>) istream)
  "Deserializes a message object of type '<handle_info>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'left_x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'left_y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'right_x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'right_y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'left_putter_a) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'right_putter_b) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'left_button_vra) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'right_button_vrb) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'connected) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<handle_info>)))
  "Returns string type for a message object of type '<handle_info>"
  "arm_pkg/handle_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'handle_info)))
  "Returns string type for a message object of type 'handle_info"
  "arm_pkg/handle_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<handle_info>)))
  "Returns md5sum for a message object of type '<handle_info>"
  "508756f475519ffbaa213660eb366a74")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'handle_info)))
  "Returns md5sum for a message object of type 'handle_info"
  "508756f475519ffbaa213660eb366a74")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<handle_info>)))
  "Returns full string definition for message of type '<handle_info>"
  (cl:format cl:nil "float64 left_x~%float64 left_y~%float64 right_x~%float64 right_y~%int8  left_putter_a~%int8  right_putter_b~%float64 left_button_vra~%float64 right_button_vrb~%int8 connected~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'handle_info)))
  "Returns full string definition for message of type 'handle_info"
  (cl:format cl:nil "float64 left_x~%float64 left_y~%float64 right_x~%float64 right_y~%int8  left_putter_a~%int8  right_putter_b~%float64 left_button_vra~%float64 right_button_vrb~%int8 connected~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <handle_info>))
  (cl:+ 0
     8
     8
     8
     8
     1
     1
     8
     8
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <handle_info>))
  "Converts a ROS message object to a list"
  (cl:list 'handle_info
    (cl:cons ':left_x (left_x msg))
    (cl:cons ':left_y (left_y msg))
    (cl:cons ':right_x (right_x msg))
    (cl:cons ':right_y (right_y msg))
    (cl:cons ':left_putter_a (left_putter_a msg))
    (cl:cons ':right_putter_b (right_putter_b msg))
    (cl:cons ':left_button_vra (left_button_vra msg))
    (cl:cons ':right_button_vrb (right_button_vrb msg))
    (cl:cons ':connected (connected msg))
))
