#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H

#include <cassert>
#include <typeinfo>

namespace Arboria {

	enum CVariableFlags {
		CVAR_ALL = -1,
		CVAR_BOOL = 1 << 0,
		CVAR_INTEGER = 1 << 1,
		CVAR_FLOAT = 1 << 2,
		CVAR_ENGINE = 1 << 3,
		CVAR_RENDERER = 1 << 4,
		CVAR_SOUND = 1 << 5,
		CVAR_GUI = 1 << 6,
		CVAR_GAME = 1 << 7,
		CVAR_STATIC = 1 << 8,
		CVAR_FROMUSER = 1 << 9,
		CVAR_FROMENGINE = 1 << 10,
		CVAR_CMDONLY = 1 << 11,
		CVAR_READONLY = 1 << 12,
		CVAR_CONFIG = 1 << 13,
		CVAR_MODIFIED = 1 << 14
	};

	class CVariable {
		public:
			CVariable() { assert(typeid(this) != typeid(CVariable)); }
			inline CVariable(const char* _name, const char* _value, int _flags, const char* _description) { init(_name, _value, _flags, _description, -1, -1); }
			inline CVariable(const char* _name, const char* _value, int _flags, const char* _description, float _varMin, float _varMax) { init(_name, _value, _flags, _description, _varMin, _varMax); }
			virtual ~CVariable() {};
			inline const char* getName() const { return internalVar->name; }
			inline int getFlags() const { return internalVar->flags; }
			inline float getMinValue() const { return internalVar->minValue; }
			inline float getMaxValue() const { return internalVar->maxValue; }
			inline bool isModified() const { return (internalVar->flags & CVAR_MODIFIED) != 0; }
			inline void setModified() { internalVar->flags |= CVariableFlags::CVAR_MODIFIED; }
			inline void clearModified() { internalVar->flags &= ~CVAR_MODIFIED; }
			inline const char* getString() const { return internalVar->value; }
			inline int getInteger() const { return internalVar->intValue; }
			inline float getFloat() const { return internalVar->floatValue; }
			inline bool getBool() const { return internalVar->intValue != 0; }
			inline void setString(const char* value) { internalVar->setStringInternal(value); }
			inline void setBool(bool value) { internalVar->setBoolInternal(value); }
			inline void setInteger(int value) { internalVar->setIntegerInternal(value); }
			inline void setFloat(float value) { internalVar->setFloatInternal(value); }
			inline void setInternalVar(CVariable* cvar) { internalVar = cvar; }

			inline bool isInteger() const { return (internalVar->flags & CVAR_INTEGER); }
			inline bool isFloat() const { return (internalVar->flags & CVAR_FLOAT); }
			inline bool isBool() const { return (internalVar->flags & CVAR_BOOL); }
		protected:
			const char* name;
			const char* value;
			int intValue;
			float floatValue;
			const char* description;
			int flags;
			float minValue;
			float maxValue;
			CVariable* internalVar;
		private:
			void init(const char* _name, const char* _value, int _flags, const char* _description, float valueMin, float valueMax);
			virtual void setStringInternal(const char* newValue) {}
			virtual void setBoolInternal(bool newValue) {}
			virtual void setIntegerInternal(int newValue) {}
			virtual void setFloatInternal(float newValue) {}
	};

	class CVariableBool : public CVariable {
		CVariableBool(const char* _name, const char* _value, int _flags, const char* _description) : CVariable(_name, _value, _flags | CVAR_BOOL, _description) {}
		operator bool() { return getBool(); }
		void operator = (bool _value) { setBool(_value); }
	};

	class CVariableInt : public CVariable {
		CVariableInt(const char* _name, const char* _value, int _flags, const char* _description) : CVariable(_name, _value, _flags | CVAR_INTEGER, _description) {}
		operator int() { return getInteger(); }
		void operator = (int _value) { setInteger(_value); }
	};

	class CVariableFloat : public CVariable {
		CVariableFloat(const char* _name, const char* _value, int _flags, const char* _description) : CVariable(_name, _value, _flags | CVAR_FLOAT, _description) {}
		operator float() { return getFloat(); }
		void operator = (float _value) { setFloat(_value); }
	};

	class CVariableSystem {
		public:
			virtual ~CVariableSystem() {};
			virtual void init() = 0;
			virtual void shutdown() = 0;
			virtual bool isInitialized() const = 0;
			virtual void registerVariable(CVariable* var) = 0;
			virtual CVariable* find(const char* name) = 0;
			virtual void setVariableString(const char* name, const char* value, int flags = 0) = 0;
			virtual void setVariableBool(const char* name, const bool value, int flags = 0) = 0;
			virtual void setVariableInteger(const char* name, const int value, int flags = 0) = 0;
			virtual void setVariableFloat(const char* name, const float value, int flags = 0) = 0;

			virtual void setModifiedFlags(int flags) = 0;
			virtual int getModifiedFlags() const = 0;
			virtual void clearModifiedFlags(int flags) = 0;
			virtual void resetFlaggedVariables(int flags) = 0;

			virtual const char* getVariableString(const char* name) const = 0;
			virtual bool getVariableBool(const char* name) const = 0;
			virtual int getVariableInteger(const char* name) const = 0;
			virtual float getVariableFloat(const char* name) const = 0;
	};
	extern CVariableSystem* cVariableSystem;
}

#endif