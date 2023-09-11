(defun print_answer (Sys) (print_answer1 (transform_equations (make_equation_all (Sys_Ans Sys))) Sys))

(defun print_answer1 (Solved_Sys Sys)
    (cond ((eql Solved_Sys 'Error) (print "SYSTEM HAS NO SOLUTION") (print '--------------------------) Nil)
          (T (print_answer2 Solved_Sys Sys))
    )
)

(defun print_answer2 (Solved_Sys Sys)
    (cond ((print_free_vars (free_vars_list (not_free_vars_list Solved_Sys) (all_vars_sys (into_inner Sys)))) (print_answer3 (into_outer Solved_Sys)))
          (T Nil)
    )
)

(defun print_free_vars (Free_List)
    (cond ((null Free_List) T)
          (T (print "List of free variables") (print Free_List) T)
    )
)

(defun print_answer3 (Solved_Sys)
    (cond ((null Solved_Sys) (print '--------------------------))
          (T (print (car Solved_Sys)) (print_answer3 (cdr Solved_Sys)))
    )
)

(defun into_outer (Solved_Sys)
    (cond ((null Solved_Sys) Nil)
          (T (cons (into_outer_line (car Solved_Sys)) (into_outer (cdr Solved_Sys))))
    )
)

(defun into_outer_line (Solved_Line)
    (cond ((null Solved_Line) Nil)
          ((atom (car Solved_Line)) (cond ((null (cddr Solved_Line)) (cons (car Solved_Line) (cons '= (cons (cadadr Solved_Line) Nil))))
                                          (T (cons (car Solved_Line) (cons '= (cons (cadadr Solved_Line) (cons '* (cons (caadr Solved_Line) (into_outer_line (cddr Solved_Line))))))))
                                    ))
          ((eql (caar Solved_Line) '=) (cond ((< (cadar Solved_Line) 0) (cons '- (cons (* -1 (cadar Solved_Line)) (into_outer_line (cdr Solved_Line)))))
                                             ((> (cadar Solved_Line) 0) (cons '+ (cons (cadar Solved_Line) (into_outer_line (cdr Solved_Line)))))
                                             (T  (into_outer_line (cdr Solved_Line)))
                                       ))
          (T (cond ((< (cadar Solved_Line) 0) (cons '- (cons (* -1 (cadar Solved_Line)) (cons '* (cons (caar Solved_Line) (into_outer_line (cdr Solved_Line)))))))
                   ((> (cadar Solved_Line) 0) (cons '- (cons (cadar Solved_Line) (cons '* (cons (caar Solved_Line) (into_outer_line (cdr Solved_Line)))))))
                   (T  (into_outer_line (cdr Solved_Line)))
             ))
    )
)

(defun check_nil (Solved_Sys)
    (cond ((null Solved_Sys) Nil)
          ((equal (car Solved_Sys) Nil) T)
          (T (check_nil (cdr Solved_Sys)))
    )
)

;Преобразует выражение переменной в уравнение (для обратного хода)
(defun transform_equations (Equations)
    (cond ((null Equations) Nil)
          ((equal Equations 'Error) 'Error)
          (T (transform_equations1 Equations Nil))
    )
)

(defun transform_equations1 (Equations Res)
    (cond ((null Equations) Res)
          (T (transform_equations1 (transform_all (make_statement (car Equations)) (cdr Equations)) (cons (make_statement (car Equations)) Res)))
    )
)




;---------------------------ПОЛУЧАЕМ ВЫРАЖЕНИЯ ДЛЯ ВСЕХ ПЕРЕМЕННЫХ ЧЕРЕЗ ДРУГИЕ--------------------------------------------------------------------

(defun Sys_Ans (Sys) (Solve (pre_transform (into_inner Sys))))

(defun Solve (Sys) (Solve1 Sys Nil))

(defun Solve1 (Sys Res)
    (cond ((null Sys) Res)
          ((equal Sys 'Error) 'Error)
          ((check_nil (transform_all (make_statement (car Sys)) (cdr Sys)))  'Error)
          (T (Solve1 (transform_all (make_statement (car Sys)) (cdr Sys)) (cons (make_statement (car Sys)) Res)))
    )
)

;--------------------------------------------------------------------------------------------------------------------------------------------------



;--------------------------БЛОК ФУНКЦИЙ ДЛЯ ПРЕОБРАЗОВАНИЯ УРАВНЕНИЙ ПОСЛЕ ИСКЛЮЧЕНИЯ ОЧЕРЕДНОЙ ПЕРЕМЕННОЙ-----------------------------------------

(defun transform_all (Statement Sub_Sys)
    (cond ((null Sub_Sys) Nil)
          (T (cond ((check_identity (transform1 (car Statement) (cdr Statement) (car Sub_Sys))) (transform_all Statement (cdr Sub_Sys)))
                   ((check_wrong (transform1 (car Statement) (cdr Statement) (car Sub_Sys))) '(Nil))
                   (T (cons (transform1 (car Statement) (cdr Statement) (car Sub_Sys)) (transform_all Statement (cdr Sub_Sys))))
             )
          )
    
    )
)

(defun check_identity (Equation)
    (cond ((null Equation) T)
          ((= (cadar Equation) 0) (check_identity (cdr Equation)))
          (T Nil)
    )
)

(defun check_wrong (Equation)
    (cond ((null Equation) T)
          ((eql (caar Equation) '=) (/= 0 (cadar Equation)))
          ((= (cadar Equation) 0) (check_wrong (cdr Equation)))
          (T Nil)
    )
)

(defun transform1 (Var Statement Equation); Преобразует уравнение Equation в соответствии с выражением Statement для переменной Var 
    (cond ((not (check_in Var Equation)) Equation)
          (T (del_0 (transform4 Statement (transform2 Var Equation) (transform3 Var Equation))))
    )
)

(defun transform2 (Var Equation); Возвращает уравнение Equation без переменной Var
    (cond ((eql Var (caar Equation)) (cdr Equation))
          (T (cons (car Equation) (transform2 Var (cdr Equation))))
    )
    
)

(defun transform3 (Var Equation); Возвращает коэффициент переменной Var в уравнении Equation
    (cond ((eql Var (caar Equation)) (cadar Equation))
          (T (transform3 Var (cdr Equation)))
    )
)

(defun transform4 (Statement Equation Coef); Прееобразует уравнение Equation, исключая переменную с соответствующим выражением Statement. Для этого
; ей также нужен коэффициент Coef перед исключаемой переменной в уравнении Equation
    (cond ((null Statement) Equation)
          (T (transform4 (cdr Statement) (transform5 (caar Statement) (cadar Statement) Equation Coef) Coef))
    )
)

; Ищет в уравнении коэффициент при переменной Var и меняет его в соответствии с коэффициентом в выражении
;Var - очередная переменная, у которой меняем коэффициент, Var_Coef - коэффициент перед переменной Var в выражении Statement, Equation - уравнени,
;в котором мы преобразуем коэффициент перед переменной Var, Coef - это коэффициент перед переменной, которую мы исключили из Equation
(defun transform5 (Var Var_Coef Equation Coef)
    (cond ((check_in Var Equation) (transform5_1 Var Var_Coef Equation Coef))
          (T (transform5_2 Var Var_Coef Equation Coef))
    )
)

(defun transform5_1 (Var Var_Coef Equation Coef)
    (cond ((eql Var (caar Equation)) (cons (cons Var (cons (+ (* Var_Coef Coef) (cadar Equation)) Nil)) (cdr Equation)))
          ((null Equation) (cons (cons Var (cons (* Var_Coef Coef) Nil)) (cdr Equation)))
          (T (cons (car Equation) (transform5 Var Var_Coef (cdr Equation) Coef)))
    )
)

(defun transform5_2 (Var Var_Coef Equation Coef)
    (cons (cons Var (cons (* Var_Coef Coef) Nil)) Equation)
)

(defun check_in (Var Equation); Проверяет, есть ли в уравнении Equation переменная Var, которую мы хотим исключить
    (cond ((null Equation) Nil)
          ((eql Var (caar Equation)) T)
          (T (check_in Var (cdr Equation)))
    )
)
 
(defun del_0 (Equation); Убираем из уравнения переменные с коэффициентом 0, но если уравнение является неверным тождеством, то обрабатываем его позднее
    (cond ((check_wrong Equation) Equation)
          (T (del_1 Equation))
    )
)

(defun del_1 (Equation)
    (cond ((null Equation) Nil)
          ((and (= 0 (cadar Equation)) (not (eql '= (caar Equation)))) (del_1 (cdr Equation)))
          (T (cons (car Equation) (del_1 (cdr Equation))))
    )
)
;-------------------------------------------------------------------------------------------------------------------------------------------------------



(defun make_statement (Line) (cons (caar Line) (make_statement1 (cdr Line) (cadar Line)))); Выражает самую первую переменную в уравнении через другие

(defun make_statement1(Line Coef); Вспомогательная функция для функции make_statement
    (cond ((null Line) Nil)
          (T (cons (cons (caar Line) (cons (- 0 (/ (cadar Line) coef)) Nil)) (make_statement1 (cdr Line) Coef)))

    )
)

(defun make_equation_all (Statements)
    (cond ((null Statements) Nil)
          ((equal Statements 'Error) 'Error)
          (T (cons (make_equation (car Statements)) (make_equation_all (cdr Statements))))
    )
)

(defun make_equation (Statement) (cons (cons (car Statement) (cons '1 Nil)) (make_equation1 (cdr Statement))))

(defun make_equation1 (Statement)
    (cond ((null Statement) Nil)
          (T (cons (cons (caar Statement) (cons (- 0 (cadar Statement)) Nil)) (make_equation1 (cdr Statement))))

    )
)


(defun into_inner (Sys) 
    (cond ((null Sys) Nil)
          (T (cons (into_inner_line (car Sys)) (into_inner (cdr Sys))))
    )
)
(defun into_inner_line (Line) (into_inner_line1 Line '((= 0)) 1))
(defun into_inner_line1 (Line Res Sign)
    (cond ((null Line) Res)
          ((symbolp (car Line))
                (cond ((eql (car Line) '+ ) 
                    (cond ((symbolp (cadr Line)) (into_inner_line1 (cddr Line) (del_dublicates (cadr Line) (* Sign 1) Res) Sign))
                          (T 
                                (cond ((eql '* (caddr Line)) (into_inner_line1 (cddddr Line) (del_dublicates (cadddr Line) (* Sign (cadr Line)) Res) Sign))
                                      (T (into_inner_line1 (cddr Line) (del_dublicates '= (* Sign (cadr Line)) Res) Sign))
                                )
                          
                          )
                    ))
                      ((eql (car Line) '-)
                    (cond ((symbolp (cadr Line)) (into_inner_line1 (cddr Line) (del_dublicates (cadr Line) (* Sign -1) Res) Sign))
                          (T 
                                (cond ((eql '* (caddr Line)) (into_inner_line1 (cddddr Line) (del_dublicates (cadddr Line) (* Sign (* -1 (cadr Line))) Res) Sign))
                                      (T (into_inner_line1 (cddr Line) (del_dublicates '= (* -1 (cadr Line)) Res) Sign))
                                )
                          
                          )
                    ))
                      ((eql (car Line) '=) (into_inner_line1 (cdr Line) Res -1))
                      (T (into_inner_line1 (cdr Line) (del_dublicates (car Line) (* Sign 1) Res) Sign))
                ))
          (T 
                (cond ((eql '* (cadr Line)) (into_inner_line1 (cdddr Line) (del_dublicates (caddr Line) (* Sign (car Line)) Res) Sign))
                      (T (into_inner_line1 (cdr Line) (del_dublicates '= (* Sign (car Line)) Res) Sign))
                )
          ) 
    )
)




(defun del_dublicates (Var Value Cur_Line)
    (cond ((not (member_in_Sys Var Cur_Line)) (cons (cons Var (cons Value Nil)) Cur_Line))
          (T (del_dublicates1 Var Value Cur_Line))
    )
)

(defun del_dublicates1 (Var Value Cur_Line)
    (cond ((null Cur_Line) Nil)
          ((eql Var (caar Cur_Line)) (cons (cons Var (cons (+ (cadar Cur_Line) Value) Nil)) (del_dublicates1 Var Value (cdr Cur_Line))))
          (T (cons (cons (caar Cur_Line) (cons (cadar Cur_Line) Nil)) (del_dublicates1 Var Value (cdr Cur_Line))))
    )
)

(defun pre_transform (Sys)
    (cond ((check_nil (pre_transform1 Sys)) 'Error)
          (T (pre_transform1 Sys))
    )
)

(defun pre_transform1 (Sys)
    (cond ((null Sys) Nil)
          ((check_identity (car Sys)) (pre_transform1 (cdr Sys)))
          ((check_wrong (car Sys)) '(Nil))
          (T (cons (pre_transform_line (car Sys)) (pre_transform1 (cdr Sys))))
    )
)

(defun pre_transform_line (Sys_Line)
    (cond ((null Sys_Line) Nil)
          ((and (= (cadar Sys_Line) 0) (not (eql '= (caar Sys_Line)))) (pre_transform_line (cdr Sys_Line)))
          (T (cons (cons (caar Sys_Line) (cons (cadar Sys_Line) Nil)) (pre_transform_line (cdr Sys_Line))))
    )
)


;----------------------Блок Функций, нужный для вывода свободных переменных-----------------------

(defun member_in_sys (El Sys)
    (cond ((null Sys) Nil)
          ((eql (caar Sys) El) T)
          (T (member_in_sys El (cdr Sys)))
    )
)

(defun all_vars_line (Sys_Line)
    (cond ((null Sys_Line) Nil)
          ((null (cdr Sys_Line)) Nil)
          ((member_in_sys (caar Sys_Line) (cdr Sys_Line)) (all_vars_line (cdr Sys_Line)))
          (T (cons (caar Sys_Line) (all_vars_line (cdr Sys_Line))))
    
    )
)

(defun all_vars_sys (Sys)
    (cond ((null Sys) Nil)
          (T (unite (all_vars_line (car Sys)) (all_vars_sys (cdr Sys))))
    )
)

(defun unite (Set1 Set2)
    (cond ((null Set1) Set2)
          ((not (member (car Set1) Set2)) (cons (car Set1) (unite (cdr Set1) Set2)))
          (T (unite (cdr Set1) Set2))
    )
)

(defun not_free_vars_list (Solved_Sys)
    (cond ((null Solved_Sys) Nil)
          (T (cons (caar Solved_Sys) (not_free_vars_list (cdr Solved_Sys))))
    )
)

(defun free_vars_list (Not_Free_Vars All_Vars)
    (cond ((null All_Vars) Nil)
          ((member (car All_Vars) Not_Free_Vars) (free_vars_list Not_Free_Vars (cdr All_Vars)))
          (T (cons (car All_Vars) (free_vars_list Not_Free_Vars (cdr All_Vars))))
    )
)


(print_answer '((2 * X - Y + 3 * Z = 4) (X + Y + Z = 4)  (X - 2 * Y + 2 * Z = 0)))
(print_answer '((X + 2 * Y + Z = 4) (X + 2 * Y + Z = 4)))
(print_answer '((X - X + Y - Y + Z - Z = 0)))
(print_answer '((2 + Z + Y + 2 = 5 - Z + 2 * Z + X + 3)))
(print_answer '((2 + 2 * Z + 4 + X - X + Y + 2 = 3 - Z + 2 * Z + Z + 4 + X + 2 + 3)))
(print_answer '((X = - X + 2 + 0)))





(print_answer '((X + 2 * Y + Z = 4) (X + 2 * Y + 3 * Z = 3)))
(print_answer '((X = 4) (X + Y = 3) (2 * X + Y = 7)))
(print_answer '((X + Y + 2 * Z + 3 * C = 1) (X + 2 * Y + 3 * Z - C = -4) (3 * X - Y - Z - 2 * C = -4) (2 * X + 3 * Y - Z - C = -6)))
(print_answer '((X + Y + 2 * Z + 3 * C = 1) (X + 2 * Y + 3 * Z - C = -4) (3 * X - Y - Z - 2 * C = -4) (2 * X + 3 * Y + 5 * Z + 2 * C = -3)))
(print_answer '((X + Y + 2 * Z + 3 * C = 1) (X + 2 * Y + 3 * Z - C = -4) (3 * X - Y - Z - 2 * C = -4) (4 * X + Y + 2 * Z - 3 * C = -9)))
(print_answer '((2 * X + X = 3) (X - X + Y - Y = 0)))
(print_answer '((X - X = 0) (Y - Y = 0) (Z = 1)))

(print_answer '((X - X + Y - Y = 0) (2 * X + X = 3)))
(print_answer '((X + Y = 3) (X + Y = 3)))
(print_answer '((X - X + Y - Y = 0)))
(print_answer '((0 * X = 0) (Y - Y = 0) (Z - Z = 0)))
(print_answer '((X + Y + Z = 5) (10 * X + 10 * Z + 10 * Y = 50)))









