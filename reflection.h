/*
 * REFLECTION_DECLARE_V_I(SomeClass, SomeMethod) will expand to
 * void (*__REFLECTION__SomeClass_SomeMethod_V_I)(int)
 *
 * REFLECTION_ASSIGN_V_I(SomeClass, SomeMethod, Address) will expand to
 * __REFLECTION__SomeClass_SomeMethod_V_I = reinterpret_cast<void (*)(int)>(Address + 0x3)
 *
 * REFLECT_V_I(SomeClass, SomeMethod) will expand to
 * void SomeClass::SomeMethod(int) {
 *     asm ("jmp *__REFLECTION__SomeClass_SomeMethod_V_I");
 * }
 * 
 * Note on jmp/ret: Since we jmp into a function that will ret, our dummy functions don't have their own ret
 * However the compiler complains so tell it to shut up
 * #pragma GCC diagnostic ignored "-Wreturn-type"
 * #pragma GCC diagnostic ignored "-Wuninitialized"
 */

#ifndef REFLECTION_H
#define REFLECTION_H

#define REFLECTION_DECLARE(Class, Method, Return, Args, ShortReturn, ShortArgs) \
    Return (*REFLECTION(Class, Method, ShortReturn, ShortArgs))Args

#define REFLECTION_ASSIGN(Class, Method, Return, Args, ShortReturn, ShortArgs, Address) \
    REFLECTION(Class, Method, ShortReturn, ShortArgs) = reinterpret_cast<REFLECTION_TYPE(Return, Args)>(Address + 0x3)
// + 0x3 is because the three first bytes in the target function are identical in our dummy function.
// the jmp instruction will not actually be at the exact address of our dummy function, but at +0x3
// ofcourse this only works if the target function always plays by our rules...

#define REFLECTION(Class, Method, ShortReturn, ShortArgs) \
    __REFLECTION__##Class##_##Method##_##ShortReturn##_##ShortArgs

#define REFLECTION_TYPE(Return, Args) \
    Return (*)Args

#define REFLECT(Class, Method, Return, Args, ShortReturn, ShortArgs) \
    Return Class::Method Args {REFLECT_ASM(Class, Method, ShortReturn, ShortArgs);}

#define REFLECT_ASM(Class, Method, ShortReturn, ShortArgs) \
    asm ("jmp *__REFLECTION__"#Class"_"#Method"_"#ShortReturn"_"#ShortArgs)

// pointer func(int);
#define REFLECTION_DECLARE_Z_I(Class, Method, Pointer) \
    REFLECTION_DECLARE(Class, Method, Pointer, (int), Z, I)

#define REFLECTION_ASSIGN_Z_I(Class, Method, Pointer, Address) \
    REFLECTION_ASSIGN(Class, Method, Pointer, (int), Z, I, Address)

#define REFLECTION_Z_I(Class, Method) \
    REFLECTION(Class, Method, Z, I)

#define REFLECT_Z_I(Class, Method, Pointer) \
    REFLECT(Class, Method, Pointer, (int), Z, I)

// void func(int);
#define REFLECTION_DECLARE_V_I(Class, Method) \
    REFLECTION_DECLARE(Class, Method, void, (int), V, I)

#define REFLECTION_ASSIGN_V_I(Class, Method, Address) \
    REFLECTION_ASSIGN(Class, Method, void, (int), V, I, Address)

#define REFLECTION_V_I(Class, Method) \
    REFLECTION(Class, Method, V, I)

#define REFLECT_V_I(Class, Method) \
    REFLECT(Class, Method, void, (int), V, I)

// int func(void);
#define REFLECTION_DECLARE_I_V(Class, Method) \
    REFLECTION_DECLARE(Class, Method, int, (void), I, V)

#define REFLECTION_ASSIGN_I_V(Class, Method, Address) \
    REFLECTION_ASSIGN(Class, Method, int, (void), I, V, Address)

#define REFLECTION_I_V(Class, Method) \
    REFLECTION(Class, Method, I, V)

#define REFLECT_I_V(Class, Method) \
    REFLECT(Class, Method, int, (void), I, V)

// void func(float, float, float)
#define REFLECTION_DECLARE_V_FFF(Class, Method) \
    REFLECTION_DECLARE(Class, Method, void, (float, float, float), V, FFF)

#define REFLECTION_ASSIGN_V_FFF(Class, Method, Address) \
    REFLECTION_ASSIGN(Class, Method, void, (float, float, float), V, FFF, Address)

#define REFLECTION_V_FFF(Class, Method) \
    REFLECTION(Class, Method, V, FFF)

#define REFLECT_V_FFF(Class, Method) \
    REFLECT(Class, Method, void, (float, float, float), V, FFF)

// strcmp
#define REFLECTION_CMP(Class, Method, Args, Symbol) \
    strcmp(#Class"::"#Method#Args, Symbol)

#define REFLECTION_DEMANGLE(Class, Method, Args) \
    #Class"::"#Method#Args

#define REFLECTION_CMP_I(Class, Method, Symbol) \
    REFLECTION_CMP(Class, Method, (int), Symbol)

#define REFLECTION_DEMANGLE_I(Class, Method) \
    REFLECTION_DEMANGLE(Class, Method, (int))

#define REFLECTION_CMP_V(Class, Method, Symbol) \
    REFLECTION_CMP(Class, Method, (), Symbol)

#define REFLECTION_DEMANGLE_V(Class, Method) \
    REFLECTION_DEMANGLE(Class, Method, ())

#define REFLECTION_CMP_FFF(Class, Method, Symbol) \
    REFLECTION_CMP(Class, Method, (float, float, float), Symbol)

#define REFLECTION_DEMANGLE_FFF(Class, Method) \
    REFLECTION_DEMANGLE(Class, Method, (float, float, float))

bool initReflection();

#endif
