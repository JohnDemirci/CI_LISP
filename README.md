# CI_LISP

Umit Demirci

COMP232

Tasks 1 through 6

Task1:

    -> Completed all the TODOS that were asked

    -> Added Helper functions for the mathematical operations

    -> Edited the .l file and added the missing operators

    -> fixed the malloc code in the .l file

    -> successfully ran what was asked in the task1

    -> each mathematical operation was tested and the results that came were satisfying

    -> One thing that i did not realize was the rounding we had to do. After realizing that I fixed it.

    later update

        -> (div 4 0) would give me some random value instead i added isinf function now it will not return any value


Task2:

    -> Changed the program grammar in a way that it would work the way it was asked

    -> Edited the .l .y files to add the let and symbol into our grammar

    -> created new functions linkSymbolNode, addToSymbolTable, createSymbolTableNode, makeNewSymbol, findSymbol, evalSymbolNode

    -> Made adjustments to eval, createFunctionNode

    -> tested the sample input that was given on the myCi wesite.

    -> successfully ran the inputs and got satisfying results.



Task3:

    -> I added keywords double and int and made the necessary changes in the .y and .l files

    -> extended the number of parameters that createSymbolTableNode function would take so that
        i can take the user nput and make the necessary changes

    -> i checked and compared the types of the user inputs and the declared type

    -> created a preccision warning if a user inputs something like  ((let (int a 1.25))(add a 1))

Task 4:
    -> read task 5

Task5:
    -> added ( func s_expr_list ) s_expr_list ::= s_expr s_expr_list | s_expr | empty to the grammar

    -> created the *addSexprToList so that i would take multiple values

    -> created the eval binary and eval unary functions so that if user enters anything illegal it prints out a message

    -> changed the functionality of add and mult operations so that they can take multiple values

    -> I skipped task 4 and implemented it in task 5.

    -> i checked the given data on canvas and it worked

    -> one difficulty i had on this task was that it broke tasks 1 2 3.

    -> changed the order of the .y file i was getting some random warning. I had no clue how to fix but it worked somehow

Task6:

    -> has some flaws and also incomplete

    -> added the read function. It reads the user inpt however afterwards it bugs and reutns and Nan VALUE

    -> the way i set up my printRetVal ignores infinite and NaN values and it just does not return anything

    if i have something like

    > (read)
    READ :=  4

    type: Double
    value: 4.000000
    >
    >

    the first > sybol down in the bottom indicates that there is a NaN value
    the second > down in the bottom just creates the new program asking another operation


    -> when it comes to random it always gives me the same random values,
    > (rand)

    type: Double
    value: 0.000008
    > (rand)

    type: Double
    value: 0.131538
    > (rand)

    type: Double
    value: 0.755605

    the every time i ask for a random value i always get these results.

    -> cond incomplete.

